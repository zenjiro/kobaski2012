/*
 * YourPlayer.h
 *
 *  Created on: 2011/12/16
 *      Author: Yasuyuki
 */

#ifndef YOURPLAYER_H_
#define YOURPLAYER_H_

//#include "Map.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const int M=32;

void calc(const bool &);

class YourPlayer {
public:
//	ofstream logfile;
	YourPlayer() {
		count = 0;
	}
	~YourPlayer() {
//		logfile.close();
	}

	void run() {
//		Map map = Map::createMap();

		if (count % 2 == 0) {
			calc(true);
		} else {
			calc(false);
		}
		count++;
	}

//private:
	// returns random player action
//	string getRandomAction(void);
	int count;
//	void calcDist(int,int,const bool &,int [M][M],int [M][M]);
};


#endif /* YOURPLAYER_H_ */
