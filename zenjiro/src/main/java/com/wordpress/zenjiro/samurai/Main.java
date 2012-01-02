package com.wordpress.zenjiro.samurai;

import java.util.Scanner;

/**
 * メイン
 */
public class Main {
	/**
	 * メインメソッド
	 * @param args 引数
	 */
	public static void main(final String[] args) {
		final Scanner sc = new Scanner(System.in);
		final MyPlayer player = new MyPlayer();
		while (true) {
			player.run(sc);
		}
	}
}
