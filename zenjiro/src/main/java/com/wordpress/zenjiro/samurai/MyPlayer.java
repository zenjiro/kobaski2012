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
	private int _count = 0;
	private Map _map;

	// returns a player action
	private String getRandomAction() {
		switch (this._count * 11 % 5) {
		case 0:
			return "NONE";
		case 1:
			return "RIGHT";
		case 2:
			return "UP";
		case 3:
			return "LEFT";
		case 4:
			return "DOWN";
		default:
			throw new RuntimeException();
		}
	}

	public void run(final Scanner sc) {
		// TODO: ここにあなたのAIプログラムを書いてください！
		this._map = Map.createOrUpdateMap(this._map, sc);
		// for debug (you can remove this statement)
		this.log();
		if (this._count % 2 == 0) {
			System.out.println(this.getRandomAction());
		} else {
			System.out.println(this.getRandomAction());
		}
		this._count++;
		System.out.flush();
	}

	// ----------- debug code (you can remove the following code) -----------
	private BufferedWriter _writer;

	public MyPlayer() {
		try {
			final FileOutputStream fos = new FileOutputStream("log_java_jp.txt");
			this._writer = new BufferedWriter(new OutputStreamWriter(fos));
		} catch (final FileNotFoundException e) {
		}
	}

	public void log() {
		try {
			this._writer.write(this._map.getRemainingTime() + ", ");
			this._writer.write(this._map.getPlayerIndex() + ", ");
			this._writer.write(this._map.getWidth() + ", ");
			this._writer.write(this._map.getHeight() + "\r\n");
			for (final Chara chara : this._map.getAllCharas()) {
				for (int i = 0; i < 4; i++) {
					this._writer.write(this._map.isMovable(chara,
							Direction.values()[i])
							+ ", ");
				}
				this._writer.write("\r\n");
			}
			for (final Chara samurai : this._map.getAllSamurais()) {
				this._writer.write(this._map.isAvailable(samurai.getX(),
						samurai.getY()) + ", ");
				this._writer.write("{ X = " + samurai.getX() + ", Y = "
						+ samurai.getY() + " }" + "\r\n");
			}
			for (final Chara dog : this._map.getAllDogs()) {
				this._writer
						.write(this._map.isAvailable(dog.getX(), dog.getY())
								+ ", ");
				this._writer.write("{ X = " + dog.getX() + ", Y = "
						+ dog.getY() + " }" + "\r\n");
			}
			this._writer.flush();
		} catch (final IOException e) {
		}
	}
	// ----------------------------------------------------------------------
}