package com.wordpress.zenjiro.samurai;

import java.awt.Point;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Arrays;
import java.util.Scanner;

import jp.ac.waseda.cs.washi.samurai.api.Chara;
import jp.ac.waseda.cs.washi.samurai.api.CharaState;
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
			final Distance distance = this.getDistance(this.map.getMySamurai(),
					this.map.getMySamurai().getState() != CharaState.SHOGUN);
			// 最寄りのボーナスを追いかける。
			Point nearestBig = null;
			Point nearestSmall = null;
			int minBigDistance = Integer.MAX_VALUE;
			int minSmallDistance = Integer.MAX_VALUE;
			for (int i = 0; i < this.map.getHeight(); i++) {
				for (int j = 0; j < this.map.getWidth(); j++) {
					switch (this.map.getBonus(j, i)) {
					case SHOGUN:
						if (distance.distance[i][j] < minBigDistance) {
							nearestBig = new Point(j, i);
							minBigDistance = distance.distance[i][j];
						}
						break;
					case BIG:
						if (distance.distance[i][j] < minBigDistance) {
							nearestBig = new Point(j, i);
							minBigDistance = distance.distance[i][j];
						}
						break;
					case SMALL:
						if (distance.distance[i][j] < minSmallDistance) {
							nearestSmall = new Point(j, i);
							minSmallDistance = distance.distance[i][j];
						}
						break;
					}
				}
			}
			if (nearestBig != null) {
				System.out.println(this.getDirection(nearestBig.x,
						nearestBig.y, distance.path));
			} else if (nearestSmall != null) {
				System.out.println(this.getDirection(nearestSmall.x,
						nearestSmall.y, distance.path));
			} else {
				System.out.println("NONE");
			}
		} else {
			// とりあえずプレイヤ1をひたすら追いかけさせる。
			final Distance distance = this.getDistance(this.map.getMyDog(),
					false);
			final Chara target = this.map.getSamurai(1);
			if (target.getState() != CharaState.INVISIBLE) {
				System.out.println(this.getDirection(target.getX(),
						target.getY(), distance.path));
			} else {
				System.out.println("NONE");
			}
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

		@Override
		public String toString() {
			return "distance = " + Arrays.deepToString(this.distance)
					+ ", path = " + Arrays.deepToString(this.path);
		}
	}

	/**
	 * @param chara 始点とするChara
	 * @param hateDogs 敵の犬を避けるかどうか
	 * @return 指定したCharaから各地点への最短距離と経路
	 */
	private Distance getDistance(final Chara chara, boolean hateDogs) {
		final int[][] distance = new int[this.map.getHeight()][this.map
				.getWidth()];
		final Direction[][] path = new Direction[this.map.getHeight()][this.map
				.getWidth()];
		final boolean[][] dogs = new boolean[this.map.getHeight()][this.map
				.getWidth()];
		for (int i = 0; i < this.map.getHeight(); i++) {
			distance[i] = new int[this.map.getWidth()];
			Arrays.fill(distance[i], Integer.MAX_VALUE);
			path[i] = new Direction[this.map.getWidth()];
			Arrays.fill(path[i], Direction.UNKNOWN);
			dogs[i] = new boolean[this.map.getWidth()];
		}
		if (hateDogs) {
			for (Chara dog : this.map.getAllDogs()) {
				if (dog != this.map.getMyDog()) {
					dogs[dog.getY()][dog.getX()] = true;
				}
			}
		}
		distance[chara.getY()][chara.getX()] = 0;
		this.search(chara.getX(), chara.getY(), distance, path, dogs);
		return new Distance(distance, path);
	}

	/**
	 * 深さ優先で探索します。
	 * @param x 始点のx座標
	 * @param y 始点のy座標
	 * @param distance 各地点への最短距離
	 * @param path 各地点への最短経路
	 * @param dogs 敵の犬の座標
	 */
	private void search(final int x, final int y, final int[][] distance,
			final Direction[][] path, boolean[][] dogs) {
		final int d = distance[y][x] + 1;
		if (this.map.isAvailable(x, y + 1) && !this.map.isWall(x, y + 1)
				&& !dogs[y + 1][x] && d < distance[y + 1][x]) {
			distance[y + 1][x] = d;
			path[y + 1][x] = Direction.DOWN;
			this.search(x, y + 1, distance, path, dogs);
		}
		if (this.map.isAvailable(x + 1, y) && !this.map.isWall(x + 1, y)
				&& !dogs[y][x + 1] && d < distance[y][x + 1]) {
			distance[y][x + 1] = d;
			path[y][x + 1] = Direction.RIGHT;
			this.search(x + 1, y, distance, path, dogs);
		}
		if (this.map.isAvailable(x, y - 1) && !this.map.isWall(x, y - 1)
				&& !dogs[y - 1][x] && d < distance[y - 1][x]) {
			distance[y - 1][x] = d;
			path[y - 1][x] = Direction.UP;
			this.search(x, y - 1, distance, path, dogs);
		}
		if (this.map.isAvailable(x - 1, y) && !this.map.isWall(x - 1, y)
				&& !dogs[y][x - 1] && d < distance[y][x - 1]) {
			distance[y][x - 1] = d;
			path[y][x - 1] = Direction.LEFT;
			this.search(x - 1, y, distance, path, dogs);
		}
	}

	/**
	 * @param x 目的地のx座標
	 * @param y 目的地のy座標
	 * @param path 各地点への最短経路
	 * @return 指定した地点へ向かうパスの最初の方向
	 */
	private Direction getDirection(int x, int y, final Direction[][] path) {
		Direction direction = Direction.UNKNOWN;
		loop: while (true) {
			switch (path[y][x]) {
			case DOWN:
				direction = path[y][x];
				y--;
				break;
			case RIGHT:
				direction = path[y][x];
				x--;
				break;
			case UP:
				direction = path[y][x];
				y++;
				break;
			case LEFT:
				direction = path[y][x];
				x++;
				break;
			case UNKNOWN:
				break loop;
			}
		}
		return direction;
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