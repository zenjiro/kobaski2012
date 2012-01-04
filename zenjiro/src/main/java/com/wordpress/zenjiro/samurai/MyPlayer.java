package com.wordpress.zenjiro.samurai;

import java.awt.Point;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

import jp.ac.waseda.cs.washi.samurai.api.Chara;
import jp.ac.waseda.cs.washi.samurai.api.CharaState;
import jp.ac.waseda.cs.washi.samurai.api.Direction;
import jp.ac.waseda.cs.washi.samurai.api.Map;

/**
 * プレイヤの実装
 */
public class MyPlayer implements Player {
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
			return "distance = " + Arrays.deepToString(this.distance) + ", path = "
					+ Arrays.deepToString(this.path);
		}
	}

	/**
	 * @param chara 始点とするChara
	 * @param hateDogs 敵の犬を避けるかどうか
	 * @param map マップ
	 * @return 指定したCharaから各地点への最短距離と経路
	 */
	private Distance getDistance(final Chara chara, final boolean hateDogs, final Map map) {
		final int[][] distance = new int[map.getHeight()][map.getWidth()];
		final Direction[][] path = new Direction[map.getHeight()][map.getWidth()];
		final boolean[][] dogs = new boolean[map.getHeight()][map.getWidth()];
		for (int i = 0; i < map.getHeight(); i++) {
			distance[i] = new int[map.getWidth()];
			Arrays.fill(distance[i], Integer.MAX_VALUE);
			path[i] = new Direction[map.getWidth()];
			Arrays.fill(path[i], Direction.NONE);
			dogs[i] = new boolean[map.getWidth()];
		}
		if (hateDogs) {
			for (final Chara dog : map.getAllDogs()) {
				if (dog != map.getMyDog()) {
					dogs[dog.getY()][dog.getX()] = true;
				}
			}
		}
		distance[chara.getY()][chara.getX()] = 0;
		search(chara.getX(), chara.getY(), distance, path, dogs, map);
		return new Distance(distance, path);
	}

	/**
	 * 深さ優先で探索します。
	 * @param x 始点のx座標
	 * @param y 始点のy座標
	 * @param distance 各地点への最短距離
	 * @param path 各地点への最短経路
	 * @param dogs 敵の犬の座標
	 * @param map マップ
	 */
	private void search(final int x, final int y, final int[][] distance, final Direction[][] path,
			final boolean[][] dogs, final Map map) {
		final int d = distance[y][x] + 1;
		for (final Direction direction : Direction.values()) {
			if (direction != Direction.NONE) {
				if (map.isAvailable(x + direction.dx, y + direction.dy)
						&& !map.isWall(x + direction.dx, y + direction.dy)
						&& !dogs[y + direction.dy][x + direction.dx]
						&& d < distance[y + direction.dy][x + direction.dx]) {
					distance[y + direction.dy][x + direction.dx] = d;
					path[y + direction.dy][x + direction.dx] = direction;
					this.search(x + direction.dx, y + direction.dy, distance, path, dogs, map);
				}
			}
		}
	}

	/**
	 * @param x 目的地のx座標
	 * @param y 目的地のy座標
	 * @param path 各地点への最短経路
	 * @return 指定した地点へ向かうパスの最初の方向
	 */
	private Direction getDirection(int x, int y, final Direction[][] path) {
		Direction direction = Direction.NONE;
		while (path[y][x] != Direction.NONE) {
			direction = path[y][x];
			x -= direction.dx;
			y -= direction.dy;
		}
		return direction;
	}

	/**
	 * @param x x座標
	 * @param y y座標
	 * @param map マップ
	 * @return 指定した地点に行くと敵の犬に殺されるかどうか
	 */
	private boolean isKilled(final int x, final int y, final Map map) {
		for (final Chara dog : map.getAllDogs()) {
			if (dog != map.getMyDog()) {
				if (Math.abs(dog.getX() - x) < 2 && Math.abs(dog.getY() - y) < 2
						&& (dog.getX() == x || dog.getY() == y)) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @param chara キャラ
	 * @param map マップ
	 * @return 安全な向き
	 */
	private Direction getSafeDirection(final Chara chara, final Map map) {
		for (final Direction direction : new Direction[] { Direction.DOWN, Direction.RIGHT,
				Direction.UP, Direction.LEFT }) {
			if (!isKilled(chara.getX() + direction.dx, chara.getY() + direction.dy, map)) {
				return direction;
			}
		}
		return Direction.NONE;
	}

	@Override
	public Direction calc(final Map map, final boolean isSamurai) {
		if (isSamurai) {
			final Distance distance = this.getDistance(map.getMySamurai(), map.getMySamurai()
					.getState() != CharaState.SHOGUN, map);
			// 最寄りのボーナスを追いかける。
			Point nearestBig = null;
			Point nearestSmall = null;
			int minBigDistance = Integer.MAX_VALUE;
			int minSmallDistance = Integer.MAX_VALUE;
			for (int i = 0; i < map.getHeight(); i++) {
				for (int j = 0; j < map.getWidth(); j++) {
					switch (map.getBonus(j, i)) {
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
					case NONE:
						break;
					}
				}
			}
			Direction direction = Direction.NONE;
			if (nearestBig != null) {
				direction = this.getDirection(nearestBig.x, nearestBig.y, distance.path);
			} else if (nearestSmall != null) {
				direction = this.getDirection(nearestSmall.x, nearestSmall.y, distance.path);
			}
			if (map.getMySamurai().getState() == CharaState.NORMAL
					&& isKilled(map.getMySamurai().getX() + direction.dx, map.getMySamurai().getY()
							+ direction.dy, map)) {
				final Direction safeDirection = getSafeDirection(map.getMySamurai(), map);
				Logger.getAnonymousLogger().log(Level.INFO, "{0}に動く予定でしたが、敵の犬を避けるために{1}に動きました。",
						new Object[] { direction, safeDirection });
				return safeDirection;
			} else {
				return direction;
			}
		} else {
			// とりあえずプレイヤ1をひたすら追いかけさせる。
			final Distance distance = this.getDistance(map.getMyDog(), false, map);
			final Chara target = map.getSamurai(1);
			if (target.getState() != CharaState.INVISIBLE) {
				return this.getDirection(target.getX(), target.getY(), distance.path);
			} else {
				return Direction.NONE;
			}
		}
	}
}
