package com.wordpress.zenjiro.samurai;

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
