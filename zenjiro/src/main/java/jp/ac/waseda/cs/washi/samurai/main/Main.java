package jp.ac.waseda.cs.washi.samurai.main;

import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		YourPlayer player = new YourPlayer();

		while (true) {
			player.run(sc);
		}
	}
}
