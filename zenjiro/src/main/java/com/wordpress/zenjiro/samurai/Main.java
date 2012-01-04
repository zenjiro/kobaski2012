package com.wordpress.zenjiro.samurai;

import java.util.Scanner;

import jp.ac.waseda.cs.washi.samurai.api.Map;

/**
 * メイン
 */
public class Main {
	/**
	 * メインメソッド
	 * @param args 引数
	 */
	public static void main(final String[] args) {
		final Scanner scanner = new Scanner(System.in);
		Map map = null;
		boolean isSamurai = true;
		final Player player = new MyPlayer();
		while (true) {
			map = Map.createOrUpdateMap(map, scanner);
			System.out.println(player.calc(map, isSamurai).toString().replace("UNKNOWN", "NONE"));
			isSamurai = !isSamurai;
		}
	}
}
