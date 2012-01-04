package com.wordpress.zenjiro.samurai;

import jp.ac.waseda.cs.washi.samurai.api.Direction;
import jp.ac.waseda.cs.washi.samurai.api.Map;

/**
 * プレイヤのインターフェイス
 */
public interface Player {
	/**
	 * @param map マップの状態
	 * @param isSamurai 侍の番かどうか
	 * @return 進む方向
	 */
	public Direction calc(Map map, boolean isSamurai);
}
