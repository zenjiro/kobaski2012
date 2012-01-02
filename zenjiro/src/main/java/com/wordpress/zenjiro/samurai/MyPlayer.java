package com.wordpress.zenjiro.samurai;

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Arrays;
import java.util.Scanner;

import jp.ac.waseda.cs.washi.samurai.api.Chara;
import jp.ac.waseda.cs.washi.samurai.api.Direction;
import jp.ac.waseda.cs.washi.samurai.api.Map;

/**
 * プレイヤの実装
 */
public class MyPlayer {
	/**
	 * マップ
	 */
	private Map map;
	/**
	 * 侍の番かどうか
	 */
	private boolean isSamurai;

	/**
	 * 自分の番の時に呼び出されます。
	 * @param sc スキャナ
	 */
	public void run(final Scanner sc) {
		this.map = Map.createOrUpdateMap(this.map, sc);
		this.log();
		if (this.isSamurai) {
			final Chara samurai = this.map.getMySamurai();
			// 敵の犬が隣に来たら逃げる
			Chara nearestDog = null;
			int minDistance = Integer.MAX_VALUE;
			for (final Chara dog : this.map.getAllDogs()) {
				if (dog != this.map.getMyDog()) {
					final int distance = Math.abs(dog.getX()
							- this.map.getMySamurai().getX())
							+ Math.abs(dog.getY()
									- this.map.getMySamurai().getY());
					if (distance < minDistance) {
						nearestDog = dog;
						minDistance = distance;
					}
				}
			}
			if (minDistance < 5) {
				if (this.map.isMovable(samurai, Direction.DOWN)
						&& nearestDog.getY() <= samurai.getY()) {
					System.out.println("DOWN");
				} else if (this.map.isMovable(samurai, Direction.UP)
						&& nearestDog.getY() >= samurai.getY()) {
					System.out.println("UP");
				} else if (this.map.isMovable(samurai, Direction.RIGHT)
						&& nearestDog.getX() <= samurai.getX()) {
					System.out.println("RIGHT");
				} else if (this.map.isMovable(samurai, Direction.LEFT)
						&& nearestDog.getX() >= samurai.getX()) {
					System.out.println("LEFT");
				} else {
					System.out.println("NONE");
				}
			} else {
				System.out.println("NONE");
			}
		} else {
			System.out.println("NONE");
		}
		this.isSamurai = !this.isSamurai;
	}

	/**
	 * 各地点への最短距離と経路
	 */
	static class Distance {
		/**
		 * 各地点への最短距離
		 * distance[y][x]
		 */
		final int[][] distance;
		/**
		 * 各地点への最短経路
		 * 1つ手前の地点からその地点へ入る向き
		 * path[y][x]
		 */
		final Direction[][] path;

		/**
		 * コンストラクタ
		 * @param distance 各地点への最短距離
		 * @param path 各地点への最短経路
		 */
		public Distance(final int[][] distance, final Direction[][] path) {
			this.distance = distance;
			this.path = path;
		}
	}

	/**
	 * @param chara 始点とするChara
	 * @return 指定したCharaから各地点への最短距離と経路
	 */
	private Distance getDistance(final Chara chara) {
		int[][] distance = new int[this.map.getHeight()][this.map.getWidth()];
		Direction[][] path = new Direction[this.map.getHeight()][this.map
				.getWidth()];
		for (int i = 0; i < this.map.getHeight(); i++) {
			distance[i] = new int[this.map.getWidth()];
			Arrays.fill(distance[i], Integer.MAX_VALUE);
			path[i] = new Direction[this.map.getWidth()];
			Arrays.fill(path[i], Direction.UNKNOWN);
		}
		search(chara.getX(), chara.getY(), distance, path);
		return new Distance(distance, path);
	}

	/**
	 * 再帰的に探索します。
	 * @param x 始点のx座標
	 * @param y 始点のy座標
	 * @param distance 各地点への最短距離
	 * @param path 各地点への最短経路
	 */
	private void search(int x, int y, int[][] distance, Direction[][] path) {
	}

	/**
	 * ログ出力用のライタ
	 */
	private BufferedWriter writer;

	/**
	 * コンストラクタ
	 */
	public MyPlayer() {
		this.isSamurai = true;
		try {
			final FileOutputStream fos = new FileOutputStream("log_java_jp.txt");
			this.writer = new BufferedWriter(new OutputStreamWriter(fos));
		} catch (final FileNotFoundException e) {
		}
	}

	/**
	 * ログを記録します。
	 */
	public void log() {
		try {
			this.writer.write(this.map.getRemainingTime() + ", ");
			this.writer.write(this.map.getPlayerIndex() + ", ");
			this.writer.write(this.map.getWidth() + ", ");
			this.writer.write(this.map.getHeight() + "\r\n");
			for (final Chara chara : this.map.getAllCharas()) {
				for (int i = 0; i < 4; i++) {
					this.writer.write(this.map.isMovable(chara,
							Direction.values()[i])
							+ ", ");
				}
				this.writer.write("\r\n");
			}
			for (final Chara samurai : this.map.getAllSamurais()) {
				this.writer.write(this.map.isAvailable(samurai.getX(),
						samurai.getY())
						+ ", ");
				this.writer.write("{ X = " + samurai.getX() + ", Y = "
						+ samurai.getY() + " }" + "\r\n");
			}
			for (final Chara dog : this.map.getAllDogs()) {
				this.writer.write(this.map.isAvailable(dog.getX(), dog.getY())
						+ ", ");
				this.writer.write("{ X = " + dog.getX() + ", Y = " + dog.getY()
						+ " }" + "\r\n");
			}
			this.writer.flush();
		} catch (final IOException e) {
		}
	}
}