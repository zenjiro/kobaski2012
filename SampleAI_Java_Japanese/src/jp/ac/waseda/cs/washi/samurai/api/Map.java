package jp.ac.waseda.cs.washi.samurai.api;

import java.util.ArrayList;
import java.util.Scanner;

public class Map {

	private final int _index;

	private int _remainingTime;
	private final int _width;
	private final int _height;

	private final char[][] _tiles;

	private final ArrayList<Chara> _samurais;
	private final ArrayList<Chara> _dogs;
	private final ArrayList<Chara> _charas;

	private final static int PLAYER_NUMBER = 4;

	private Map(int index, int remainingTime, int width, int height,
			ArrayList<Chara> samurais, ArrayList<Chara> dogs,
			ArrayList<Chara> charas) {
		_index = index;
		_remainingTime = remainingTime;
		_width = width;
		_height = height;
		_tiles = new char[height][width];
		_samurais = samurais;
		_dogs = dogs;
		_charas = charas;
	}

	/**
	 * 現在のマップを返します．なお，マップからゲームに関する情報をすべて取得できます．
	 * 
	 * @param map
	 *            既存のマップもしくはnull
	 * @param sc
	 *            マップデータを受け取るためのScanner
	 * @return マップ
	 */
	public static Map createOrUpdateMap(Map map, Scanner sc) {
		// 各種パラメータの読み込み
		int time = sc.nextInt();
		int index = sc.nextInt();
		int width = sc.nextInt();
		int height = sc.nextInt();
		sc.nextLine();

		if (map == null) {
			ArrayList<Chara> charas = new ArrayList<Chara>();
			ArrayList<Chara> samurais = new ArrayList<Chara>();
			ArrayList<Chara> dogs = new ArrayList<Chara>();

			for (int i = 0; i < PLAYER_NUMBER; i++) {
				Chara samurai = new Chara(CharaType.SAMURAI);
				samurais.add(samurai);
				charas.add(samurai);
				Chara dog = new Chara(CharaType.DOG);
				dogs.add(dog);
				charas.add(dog);
			}
			map = new Map(index, time, width, height, samurais, dogs, charas);
		} else {
			map._remainingTime = time;
		}

		for (int h = 0; h < map._height; h++) {
			String token = sc.nextLine();
			for (int w = 0; w < map._width; w++) {
				map._tiles[h][w] = token.charAt(w);
			}
		}

		for (int i = 0; i < Map.PLAYER_NUMBER; i++) {
			map._samurais.get(i).updateSamurai(sc);
			map._dogs.get(i).updateDog(sc);
		}
		return map;
	}

	/**
	 * 自分のプレイヤー番号を返します．
	 * 
	 * @return 自分のプレイヤー番号
	 */
	public int getPlayerIndex() {
		return _index;
	}

	/**
	 * ゲームの残り時間を返します．
	 * 
	 * @return ゲームの残り時間
	 */
	public int getRemainingTime() {
		return _remainingTime;
	}

	/**
	 * 自分の侍を返します．
	 * 
	 * @return 自分の侍
	 */
	public Chara getMySamurai() {
		return _samurais.get(_index);
	}

	/**
	 * 自分の犬を返します．
	 * 
	 * @return 自分の犬
	 */
	public Chara getMyDog() {
		return _dogs.get(_index);
	}

	/**
	 * 指定した侍を返します．
	 * 
	 * @param index
	 *            侍を指定するインデックス
	 * @return 指定した侍
	 */
	public Chara getSamurai(int index) {
		return _samurais.get(index);
	}

	/**
	 * 指定した犬を返します．
	 * 
	 * @param index
	 *            犬を指定するインデックス
	 * @return 指定した犬
	 */
	public Chara getDog(int index) {
		return _dogs.get(index);
	}

	/**
	 * 全ての侍を返します．
	 * 
	 * @return 全ての侍
	 */
	public ArrayList<Chara> getAllSamurais() {
		return _samurais;
	}

	/**
	 * 全ての犬を返します．
	 * 
	 * @return 全ての犬
	 */
	public ArrayList<Chara> getAllDogs() {
		return _dogs;
	}

	/**
	 * 全てのキャラクタを返します．
	 * 
	 * @return 全てのキャラクタ
	 */
	public ArrayList<Chara> getAllCharas() {
		return _charas;
	}

	/**
	 * マップの横幅を返します．
	 * 
	 * @return マップの横幅
	 */
	public int getWidth() {
		return _width;
	}

	/**
	 * マップの縦幅を返します．
	 * 
	 * @return マップの縦幅
	 */
	public int getHeight() {
		return _height;
	}

	/**
	 * 指定した座標にあるボーナスを返します．
	 * 
	 * @param x
	 *            x座標
	 * @param y
	 *            y座標
	 * @return ボーナスの種類
	 */
	public BonusType getBonus(int x, int y) {
		char c = _tiles[y][x];
		switch (c) {
		case 's':
			return BonusType.SMALL;
		case 'b':
			return BonusType.BIG;
		case 'p':
			return BonusType.SHOGUN;
		default:
			return BonusType.NONE;
		}
	}

	/**
	 * 指定した座標が有効かどうかを返します． つまり，X座標が(0～width], Y座標が(0～height]かどうか判定します．
	 * 
	 * @param x
	 *            x座標
	 * @param y
	 *            y座標
	 * @return 有効な場合はtrue
	 */
	public boolean isAvailable(int x, int y) {
		return 0 <= x && x < _width && 0 <= y && y < _height;
	}

	/**
	 * 指定したキャラクタが指定した方向に移動できるか判定します．
	 * 
	 * @param chara
	 *            移動可能かどうかを判定するキャラクタ
	 * @param d
	 *            移動可能かどうかを判定する方向
	 * @return 移動できる場合はtrue
	 */
	public boolean isMovable(Chara chara, Direction d) {
		int nextX = chara.getX() + d.dx;
		int nextY = chara.getY() + d.dy;

		if (!isAvailable(nextX, nextY)) {
			return false;
		}

		// 壁がある場合は移動不可
		if (isWall(nextX, nextY)) {
			return false;
		}

		// 自分と同じタイプのキャラクタがいる場合は移動不可
		for (Chara c : chara.getType() == CharaType.SAMURAI ? getAllSamurais()
				: getAllDogs()) {
			if (c.getX() == nextX && c.getY() == nextY)
				return false;
		}
		return true;
	}

	/**
	 * 指定した座標に壁があるかどうかを返します．
	 * 
	 * @param x
	 *            x座標
	 * @param y
	 *            y座標
	 * @return　壁がある場合はtrue
	 */
	public boolean isWall(int x, int y) {
		char c = _tiles[y][x];
		return c == '*';
	}
}
