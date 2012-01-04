package jp.ac.waseda.cs.washi.samurai.api;

public enum Direction {

	/**
	 * 右方向(dx, dy) = (1, 0)
	 */
	RIGHT(1, 0),

	/**
	 * 上方向(dx, dy) = (0, -1)
	 */
	UP(0, -1),

	/**
	 * 左方向(dx, dy) = (-1, 0)
	 */
	LEFT(-1, 0),

	/**
	 * 下方向(dx, dy) = (0, 1)
	 */
	DOWN(0, 1),

	/**
	 * 不可視状態のため方向不明
	 */
	UNKNOWN(0, 0), ;

	public final int dx;
	public final int dy;

	private Direction(int x, int y) {
		this.dx = x;
		this.dy = y;
	}
}
