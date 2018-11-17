// utilities.cpp

#include "utilities.h"
#include "CurlingSimulator.h"
#include "strategy.h"

#include <iostream>

using namespace std;

//! get distance^2 from center of House
float getDist(float x, float y)
{
	return pow(x - TEE_X, 2) + pow(y - TEE_Y, 2);
}

//! get distance^2 of two cordinates
float getDist(float x1, float y1, float x2, float y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

//! is a Stone in House
bool isInHouse(float x, float y)
{
	if (getDist(x, y) < pow(HOUSE_R + STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
}

//! sort Shot number (rank[] = {0, 1, 2 ... 15})
//  by distance from center of House (TEEX, TEEY)
void getRanking(int *rank, const GAMESTATE* const gs, int shotNum)
{
	// init array
	for (int i = 0; i < 16; i++) {
		rank[i] = i;
	}

	// sort
	int tmp;
	for (int i = 1; i < 16; i++) {
		for (int j = i; j > 0; j--) {
			if (getDist(gs->body[rank[j]][0], gs->body[rank[j]][1]) < getDist(gs->body[rank[j - 1]][0], gs->body[rank[j - 1]][1])) {
				// swap
				tmp = rank[j];
				rank[j] = rank[j - 1];
				rank[j - 1] = tmp;
			}
			else {
				break;
			}
		}
	}
	for (int i = 1; i < 16; i++) {
		if (!isInHouse(gs->body[rank[i]][0], gs->body[rank[i]][1])) {
			rank[i] = -1;
		}
	}
}


int getPoint(int rank[16], int itsme) {
	if (rank[0] % 2 == itsme) {
		for (int s = 0; s < 16; s++) {
			if (rank[s] == -1 || rank[s] % 2 != itsme) {
				return s;
			}
		}
	}
	else {
		for (int s = 0; s < 16; s++) {
			if (rank[s] == -1 || rank[s] % 2 == itsme) {
				return -s;
			}
		}
	}
	return 0;
}