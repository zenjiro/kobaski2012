package com.wordpress.zenjiro.samurai;

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Scanner;

import jp.ac.waseda.cs.washi.samurai.api.Chara;
import jp.ac.waseda.cs.washi.samurai.api.Direction;
import jp.ac.waseda.cs.washi.samurai.api.Map;

public class MyPlayer {
	private Map map;
	private boolean isSamurai;

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
		return null;
	}

	private BufferedWriter writer;

	public MyPlayer() {
		this.isSamurai = true;
		try {
			final FileOutputStream fos = new FileOutputStream("log_java_jp.txt");
			this.writer = new BufferedWriter(new OutputStreamWriter(fos));
		} catch (final FileNotFoundException e) {
		}
	}

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