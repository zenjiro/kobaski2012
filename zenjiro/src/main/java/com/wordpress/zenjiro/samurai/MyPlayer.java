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
			if (this.map.isMovable(samurai, Direction.DOWN)) {
				System.out.println("DOWN");
			} else {
				System.out.println("NONE");
			}
		} else {
			System.out.println("NONE");
		}
		this.isSamurai = !this.isSamurai;
	}

	private BufferedWriter _writer;

	public MyPlayer() {
		this.isSamurai = true;
		try {
			final FileOutputStream fos = new FileOutputStream("log_java_jp.txt");
			this._writer = new BufferedWriter(new OutputStreamWriter(fos));
		} catch (final FileNotFoundException e) {
		}
	}

	public void log() {
		try {
			this._writer.write(this.map.getRemainingTime() + ", ");
			this._writer.write(this.map.getPlayerIndex() + ", ");
			this._writer.write(this.map.getWidth() + ", ");
			this._writer.write(this.map.getHeight() + "\r\n");
			for (final Chara chara : this.map.getAllCharas()) {
				for (int i = 0; i < 4; i++) {
					this._writer.write(this.map.isMovable(chara,
							Direction.values()[i])
							+ ", ");
				}
				this._writer.write("\r\n");
			}
			for (final Chara samurai : this.map.getAllSamurais()) {
				this._writer.write(this.map.isAvailable(samurai.getX(),
						samurai.getY())
						+ ", ");
				this._writer.write("{ X = " + samurai.getX() + ", Y = "
						+ samurai.getY() + " }" + "\r\n");
			}
			for (final Chara dog : this.map.getAllDogs()) {
				this._writer.write(this.map.isAvailable(dog.getX(), dog.getY())
						+ ", ");
				this._writer.write("{ X = " + dog.getX() + ", Y = "
						+ dog.getY() + " }" + "\r\n");
			}
			this._writer.flush();
		} catch (final IOException e) {
		}
	}
}