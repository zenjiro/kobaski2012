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

public class YourPlayer {

	private int _count = 0;
	private Map _map;

	// returns a player action
	private String getRandomAction() {
		switch (_count * 11 % 5) {
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

	public void run(Scanner sc) {
		// TODO: ここにあなたのAIプログラムを書いてください！
		_map = Map.createOrUpdateMap(_map, sc);

		// for debug (you can remove this statement)
		log();

		if (_count % 2 == 0) {
			System.out.println(getRandomAction());
		} else {
			System.out.println(getRandomAction());
		}

		_count++;
		System.out.flush();
	}

	// ----------- debug code (you can remove the following code) -----------

	private BufferedWriter _writer;

	public YourPlayer() {
		try {
			FileOutputStream fos = new FileOutputStream("log_java_jp.txt");
			_writer = new BufferedWriter(new OutputStreamWriter(fos));
		} catch (FileNotFoundException e) {
		}
	}

	public void log() {
		try {
			_writer.write(_map.getRemainingTime() + ", ");
			_writer.write(_map.getPlayerIndex() + ", ");
			_writer.write(_map.getWidth() + ", ");
			_writer.write(_map.getHeight() + "\r\n");

			for (Chara chara : _map.getAllCharas()) {
				for (int i = 0; i < 4; i++) {
					_writer.write(_map.isMovable(chara, Direction.values()[i])
							+ ", ");
				}
				_writer.write("\r\n");
			}

			for (Chara samurai : _map.getAllSamurais()) {
				_writer.write(_map.isAvailable(samurai.getX(), samurai.getY())
						+ ", ");
				_writer.write("{ X = " + samurai.getX() + ", Y = "
						+ samurai.getY() + " }" + "\r\n");
			}

			for (Chara dog : _map.getAllDogs()) {
				_writer.write(_map.isAvailable(dog.getX(), dog.getY()) + ", ");
				_writer.write("{ X = " + dog.getX() + ", Y = " + dog.getY()
						+ " }" + "\r\n");
			}
			_writer.flush();
		} catch (IOException e) {
		}
	}
	// ----------------------------------------------------------------------
}