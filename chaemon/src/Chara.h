/*
 * Chara.h
 */

#ifndef CHARA_H_
#define CHARA_H_

#include <string>

using namespace std;

class Chara {
public:
	enum CharaType {
		/**
		 * 侍
		 * Samurai
		 */
		SAMURAI,
		/**
		 * 犬
		 * Dog
		 */
		DOG,
	};

	enum Direction {
		UNKNOWN = -1,
		RIGHT,
		UP,
		LEFT,
		DOWN,
	};

	enum CharaState {
		/**
		 * 通常状態
		 * Normal state
		 */
		NORMAL,
		/**
		 * 見えない状態
		 * Invisible state
		 */
		INVISIBLE,
		/**
		 * 将軍モード状態
		 * Shogun mode state
		 */
		SHOGUN,
	};

	Chara() {
		type = Chara::SAMURAI;
		score = 0;
		x = 0;
		y = 0;
		direction = Chara::UNKNOWN;
		state = Chara::NORMAL;
		stateRemainingTime = 0;
	}

	~Chara() {}

	void initializeChara(Chara::CharaType type, int score, int x, int y, Chara::Direction direction, Chara::CharaState state = NORMAL, int stateRemainingTime = 0);

	/**
	 * キャラクタのタイプを返します。
	 * @return キャラクタのタイプ
	 */
	/**
	 * Return a type of character.
	 * @return A type of character
	 */
	Chara::CharaType getType();

	/**
	 * キャラクタの状態を返します。
	 * @return キャラクタの状態
	 */
	/**
	 * Return a state of character.
	 * @return A state of character
	 */
	Chara::CharaState getState();

	/**
	 * キャラクタの得点を返します。
	 * @return キャラクタの得点
	 */
	/**
	 * Return a score of character.
	 * @return A score of character
	 */
	int getScore();

	/**
	 * キャラクタのx座標を返します。
	 * @return x座標
	 */
	/**
	 * Return a x-coordinate of character
	 * @return A x-coordinate of character
	 */
	int getX();

	/**
	 * キャラクタのy座標を返します。
	 * @return y座標
	 */
	/**
	 * Return a y-coordinate of character
	 * @return A y-coordinate of character
	 */
	int getY();

	/**
	 * キャラクタの状態の残り時間を返します。
	 * @return キャラクタの状態の残り時間
	 */
	/**
	 * Return the remaining time of character's state.
	 * @return Remaining time of character's state
	 */
	int getStateRemainingTime();

	/**
	 * キャラクタの向きを返します。
	 * @return キャラクタの向き
	 */
	/**
	 * Return a direction of character.
	 * @return A direction of character
	 */
	Chara::Direction getDirection();

private:
	Chara::CharaType type;
	int score;
	int x;
	int y;
	Chara::Direction direction;
	Chara::CharaState state;
	int stateRemainingTime;
};

#endif /* CHARA_H_ */
