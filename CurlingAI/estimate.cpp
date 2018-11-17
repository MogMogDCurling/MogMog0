// estimate.cpp

#include "estimate.h"
#include "strategy.h"
#include "utilities.h"

using namespace std;



/**
gstmp : ゲームステージ
shotNum : 今からショットするのが第何投か0,1,...,15
itsme : 自分の石が 0 か 1 か。
　　　itsmeさんが有利ならばプラス、不利ならばマイナスの評価になる。
shotPlayer : 次のショットは0の人か1のひとか
*/
float makeEstimateOriginal(const GAMESTATE* const gstmp, int shotNum, int itsme, int shotPlayer) {
	int rank[16];
	getRanking(rank, gstmp);
	int pt = getPoint(rank, itsme);
	return 1.0f*pt;
}


