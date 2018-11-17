#pragma once

// utilities.h
#include "CurlingSimulator.h"

float getDist(float x, float y);
float getDist(float x1, float y1, float x2, float y2);
bool isInHouse(float x, float y);
void getRanking(int *rank, const GAMESTATE* const gs, int shotNum=15);
//void cerrBody(const GAMESTATE* const gs);
//void cerrRanking(int r[16]);
int getPoint(int rank[16], int itsme);