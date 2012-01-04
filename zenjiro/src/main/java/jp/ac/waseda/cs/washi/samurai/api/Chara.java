package jp.ac.waseda.cs.washi.samurai.api;

import java.awt.Point;
import java.util.Scanner;

public class Chara {

	private final CharaType _type;
	private int _score;
	private int _x;
	private int _y;
	private Direction _direction;
	private CharaState _state;
	private int _stateRemainingTime;

	public Chara(CharaType type) {
		this._type = type;
	}
	
	public void updateSamurai(Scanner sc) {
		updateChara(sc);
		this._state = CharaState.values()[sc.nextInt()];
		this._stateRemainingTime = sc.nextInt();
	}

	public void updateDog(Scanner sc) {
		updateChara(sc);
		this._state = CharaState.NORMAL;
		this._stateRemainingTime = 0;
	}

	private void updateChara(Scanner sc) {
		this._score = sc.nextInt();
		this._x = sc.nextInt();
		this._y = sc.nextInt();
		int direction = sc.nextInt();
		this._direction = direction != -1 ? Direction.values()[direction]
				: Direction.UNKNOWN;
	}

	/**
	 * キャラクタの向きを返します．
	 * 
	 * @return　キャラクタの向き
	 */
	public Direction getDirection() {
		return _direction;
	}

	/**
	 * キャラクタの現在位置の座標を返します．
	 * 
	 * @return キャラクタの現在位置の座標
	 */
	public Point getLocation() {
		return new Point(_x, _y);
	}

	/**
	 * キャラクタの得点を返します．
	 * 
	 * @return キャラクタの得点
	 */
	public int getScore() {
		return _score;
	}

	/**
	 * キャラクタの状態を返します．
	 * 
	 * @return キャラクタの状態
	 */
	public CharaState getState() {
		return _state;
	}

	/**
	 * キャラクタの状態の残り時間を返します． 0になると同時に通常状態に戻ります．
	 * 
	 * @return キャラクタの状態の残り時間
	 */
	public int getStateRemainingTime() {
		return _stateRemainingTime;
	}

	/**
	 * キャラクタのタイプを返します．
	 * 
	 * @return キャラクタのタイプ
	 */
	public CharaType getType() {
		return _type;
	}

	/**
	 * キャラクタのx座標を返します．
	 * 
	 * @return x座標
	 */
	public int getX() {
		return _x;
	}

	/**
	 * キャラクタのy座標を返します．
	 * 
	 * @return y座標
	 */
	public int getY() {
		return _y;
	}
}
