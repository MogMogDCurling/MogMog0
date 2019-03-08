#include <random>  
#include <string>
#include <ctime>

#include "strategy.h"
#include "createShot.h"
#include "utilities.h"
#include "estimate.h"


using namespace std;


const unsigned int shotVariations = 200;

class Montecarlo {
public:
	const GAMESTATE* gs;
	int shotCount;
	SHOTVEC vec[shotVariations];
	float estimate[shotVariations];
	float weight[shotVariations];
	int better[shotVariations];
	clock_t timeLimit;

	Montecarlo(const GAMESTATE* _gs, int _sc, const SHOTVEC _vec[], const float _weight[], const int _better[]) {
		gs = _gs;
		shotCount = _sc;
		for (int i = 0; i < shotCount; i++) {
			vec[i].x = _vec[i].x;
			vec[i].y = _vec[i].y;
			vec[i].angle = _vec[i].angle;
			estimate[i] = 0.0;
			weight[i] = _weight[i];
			better[i] = _better[i];
		}
		timeLimit = 108000;
	}
};

int totalRepeatNumber1, totalRepeatNumber2, totalRepeatNumber3;

void executeMontecarlo1(Montecarlo *mc) {
	SHOTVEC vectmp;
	GAMESTATE *gstmp = (GAMESTATE *)malloc(sizeof(GAMESTATE));
	clock_t start = clock(), lap = 0;
	int repeatNumber = 5;
	int thisRank[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	for (int repeat = 0; repeat < repeatNumber; repeat++) {
		for (int k = 0; k < mc->shotCount; k++) {
			mc->estimate[k] += mc->weight[k];
			int shotNum = mc->gs->ShotNum;
			int itsme = mc->gs->ShotNum % 2;
			// copy current state to temporary state
			memcpy_s(gstmp, sizeof(GAMESTATE), mc->gs, sizeof(GAMESTATE));
			getRanking(thisRank, gstmp, shotNum);
			// １投（自分）
			Simulation(gstmp, mc->vec[k], RANDOM, NULL, -1);
			mc->estimate[k] += makeEstimateOriginal(gstmp, shotNum, itsme, 1-itsme);// 評価
			shotNum++;
			if (shotNum < 16) {
				//第２投：ランダムショット（相手）
				//CreateRandomShot(gstmp, &vectmp);
				CreateSampleShot(gstmp, &vectmp);
				Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
				mc->estimate[k] += (NEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// 評価
				shotNum++;
			}
			if (shotNum < 16) {
				//第３投：ランダムショット（自分）
				//CreateSampleShot(gstmp, &vectmp);
				CreateRandomShot(gstmp, &vectmp);
				Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
				mc->estimate[k] += (NNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// 評価
				shotNum++;
			}
		}//for (int k = 0; k < shotCount; k++) {
		if (lap == 0) {
			lap = clock() - start;
			repeatNumber = (int)(mc->timeLimit / lap);
			if (repeatNumber >= 20) repeatNumber = 20;
			totalRepeatNumber1 += repeatNumber;
		}
	}
}

void executeMontecarlo2(Montecarlo *mc) {
	SHOTVEC vectmp;
	GAMESTATE *gstmp = (GAMESTATE *)malloc(sizeof(GAMESTATE));
	clock_t start = clock(), lap = 0;
	int repeatNumber = 4;
	int thisRank[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	for (int repeat = 0; repeat < repeatNumber; repeat++) {
		for (int k = 0; k < mc->shotCount; k++) {
			if (mc->better[k] < 5) {
				mc->estimate[k] += mc->weight[k];
				int shotNum = mc->gs->ShotNum;
				int itsme = mc->gs->ShotNum % 2;
				// 盤面のコピー
				memcpy_s(gstmp, sizeof(GAMESTATE), mc->gs, sizeof(GAMESTATE));
				// １投（自分）
				getRanking(thisRank, gstmp, shotNum);
				Simulation(gstmp, mc->vec[k], RANDOM, NULL, -1);
				shotNum++;
				mc->estimate[k] += makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme);// 評価
				if (shotNum < 16) {
					//第２投：ランダムショット（相手）
					//CreateRandomShot(gstmp, &vectmp);
					CreateSampleShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// 評価
				}
				if (shotNum < 16) {
					//第３投：ランダムショット（自分）
					CreateRandomShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// 評価
				}
				if (shotNum < 16) {
					//第４投：ランダムショット（相手）
					//CreateRandomShot(gstmp, &vectmp);
					CreateSampleShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// 評価
				}
				if (shotNum < 16) {
					//第５投：ランダムショット（自分）
					CreateRandomShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNNNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// 評価
				}
			}//if (better[k] < 5) {
		}//end of for (int k = 0; k < mc->shotCount; k++)
		if (lap == 0) {
			lap = clock() - start;
			repeatNumber = (int)(mc->timeLimit / lap);
			if (repeatNumber >= 20) repeatNumber = 20;
			totalRepeatNumber2 += repeatNumber;
		}
	}//end of for (int repeat = 0; repeat < repeatNumber; repeat++) 
}



clock_t remainingTime = 210000;// total time limit
clock_t remainingTimePart1 = 3000;
clock_t remainingTimePart2 = 1000;
clock_t remainingTimePart3 = 1000;
int timeLimitPercent[16] = { 28, 28, 56, 56, 104, 104, 112, 112, 128, 128, 144, 144, 128, 128, 100, 100 };

//モンテカルロをベースとしたmogmogの主部
void myMT1(const GAMESTATE* const gs, SHOTVEC *vec_ret) {

	std::cerr << "---------------------" << gs->ShotNum << endl;
	clock_t start = clock();

	if (gs->CurEnd == 0 && gs->ShotNum <= 1) {
		remainingTime = 218000;
	}

	totalRepeatNumber1 = totalRepeatNumber2 = totalRepeatNumber3 = 0;

	int remainingShot = 8 - (int)(gs->ShotNum / 2);
	int remainingEnd = gs->LastEnd - gs->CurEnd;
	if (remainingShot == 8) {
		remainingTimePart1 = (clock_t)(remainingTime / 10 * 7 / remainingEnd / 8);
		remainingTimePart2 = (clock_t)(remainingTime / 10 * 3 / remainingEnd / 8);
	}


	int stoneInHouse = 0;
	for (int stone = 0; stone < 16; stone++) {
		if (isInHouse(gs->body[stone][0], gs->body[stone][1]) || TEE_Y + HOUSE_R < gs->body[stone][1]) {
			stoneInHouse++;
		}
	}
	if (stoneInHouse == 0 && gs->ShotNum < 14) {// -> ハウスが空だったら
		CreateGuardShot(gs, 1, vec_ret);
		clock_t finish = clock();
		std::cerr << " " << (double)(finish - start) << "(ms)" << endl;
		remainingTime -= (finish - start);
		std::cerr << "---------------------" << endl;
		return;
	}

	// 5手モンテカルロ
	// 可能性のある手を羅列する。vec[]にする。
	SHOTVEC vec[shotVariations];
	// ショットの内容を表すstring変数 string shotName[] も準備しておくとよいかも。
	string shotName[shotVariations];
	// その羅列と同じ長さの評価値の配列を準備する。
	float weight[shotVariations];
	float estimate[shotVariations];
	int better[shotVariations];

	// 可能性のある手を羅列する。
	int shotCount = 0;
	if (CreateTeeShot(gs, &(vec[shotCount]))) {
		shotName[shotCount] = "a tee shot ";
		weight[shotCount] = 0.0f;
		shotCount++;
	}
	std::cerr << "n";
	for (int stone = gs->ShotNum - 1; stone >= 0; stone--) {
		if (gs->body[stone][0] > 0.0 && gs->body[stone][1] > 0.0) {
			for (int i = 0; i < 6; i++) {
				if (CreateHitAndRollShot(gs, stone, i, &(vec[shotCount]))) {
					// ショット2までは，ヒットアンドロールは自粛。
					if (gs->ShotNum >= 2) {
						switch (i) {
						case 0: shotName[shotCount] = "a hit-and-stay shot to stone " + to_string(stone); break;
						case 1: shotName[shotCount] = "a hit-and-roll shot to stone " + to_string(stone) + " from the left"; break;
						case 2: shotName[shotCount] = "a hit-and-roll shot to stone " + to_string(stone) + " from the right"; break;
						case 3: shotName[shotCount] = "a weak hit shot to stone " + to_string(stone); break;
						case 4: shotName[shotCount] = "a weak hit shot to stone " + to_string(stone) + " from the left"; break;
						case 5: shotName[shotCount] = "a weak hit shot to stone " + to_string(stone) + " from the right"; break;
						}
						if(gs->ShotNum%2==0) weight[shotCount] = OFFENSIVE_WEIGHT;
						else weight[shotCount] = DEFENSIVE_WEIGHT;
						shotCount++;
					}
				}
			}
			for (int i = 0; i < 3; i++) {
				if (stone % 2 == gs->ShotNum % 2 && gs->body[stone][1] > TEE_Y + 0.9f) {// 0.9 := HOUSE_R * 0.5
					if (CreatePromotionShot(gs, stone, i, &(vec[shotCount]))) {
						switch (i) {
						case 0: shotName[shotCount] = "a promotion shot to stone " + to_string(stone); break;
						case 1: shotName[shotCount] = "a promotion shot to stone " + to_string(stone) + " from the left"; break;
						case 2: shotName[shotCount] = "a promotion shot to stone " + to_string(stone) + " from the right"; break;
						}
						if (gs->ShotNum % 2 == 0) weight[shotCount] = DEFENSIVE_WEIGHT;
						else weight[shotCount] = OFFENSIVE_WEIGHT;
						shotCount++;
					}
				}
				if (stone % 2 != gs->ShotNum % 2) {
					if (CreateFreezeShot(gs, stone, i, &(vec[shotCount]))) {
						switch (i) {
						case 0: shotName[shotCount] = "a freeze shot to stone " + to_string(stone); break;
						case 1: shotName[shotCount] = "a freeze shot to stone " + to_string(stone) + " from the left"; break;
						case 2: shotName[shotCount] = "a freeze shot to stone " + to_string(stone) + " from the right"; break;
						}
						if (gs->ShotNum % 2 == 0) weight[shotCount] = DEFENSIVE_WEIGHT;
						else weight[shotCount] = OFFENSIVE_WEIGHT;
						shotCount++;
					}
				}
			}
			if (CreateStoneGuardShot(gs, stone, &(vec[shotCount]))) {
				shotName[shotCount] = "a guard shot to stone " + to_string(stone);
				if (gs->ShotNum % 2 == 0) weight[shotCount] = DEFENSIVE_WEIGHT;
				else weight[shotCount] = OFFENSIVE_WEIGHT;
				shotCount++;
			}
		}
	}
	for (int i = 0; i < 4; i++) {// 6->4
		std::cerr << "o";
		if (CreateDrawShot(gs, i, &(vec[shotCount]))) {
			switch (i) {
			case 0:shotName[shotCount] = "a draw shot to right-side of the house"; break;
			case 1:shotName[shotCount] = "a draw shot to left-side of the house "; break;
			case 2:shotName[shotCount] = "a draw shot at the right of the tee"; break;
			case 3:shotName[shotCount] = "a draw shot at the left of the tee"; break;
			}
			weight[shotCount] = 0.0f;
			shotCount++;
		}
	}
	for (int i = 0; i < 2; i++) {//4->2
		std::cerr << "w";
		if (CreateGuardShot(gs, i, &(vec[shotCount]))) {
			switch (i) {
			case 0:shotName[shotCount] = "a center guard shot"; break;
			case 1:shotName[shotCount] = "a tight guard shot"; break;
			}
			weight[shotCount] = 0.0f;
			shotCount++;
		}
	}
	std::cerr << " ";
	int itsme = gs->ShotNum % 2;

	for (int k = 0; k < shotCount; k++) {
		estimate[k] = 0.0f;
		better[k] = 0;
	}
	std::cerr << "t";

	clock_t lap1 = clock();

	Montecarlo *mc11 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc12 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc13 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc14 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc15 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc16 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc17 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc18 = new Montecarlo(gs, shotCount, vec, weight, better);

	std::cerr << "h";
	mc11->timeLimit = mc12->timeLimit = mc13->timeLimit = mc14->timeLimit =
		(clock_t)(remainingTimePart1 * timeLimitPercent[gs->ShotNum] / 100);// 
	mc15->timeLimit = mc16->timeLimit = mc17->timeLimit = mc18->timeLimit =
		(clock_t)(remainingTimePart1 * timeLimitPercent[gs->ShotNum] / 100);// 

	thread thread11(bind(executeMontecarlo1, mc11));
	thread thread12(bind(executeMontecarlo1, mc12));
	thread thread13(bind(executeMontecarlo1, mc13));
	thread thread14(bind(executeMontecarlo1, mc14));
	thread thread15(bind(executeMontecarlo1, mc15));
	thread thread16(bind(executeMontecarlo1, mc16));
	thread thread17(bind(executeMontecarlo1, mc17));
	thread thread18(bind(executeMontecarlo1, mc18));

	std::cerr << "i";

	thread11.join();
	thread12.join();
	thread13.join();
	thread14.join();
	thread15.join();
	thread16.join();
	thread17.join();
	thread18.join();

	std::cerr << "n";

	for (int k = 0; k < shotCount; k++) {
		estimate[k] = mc11->estimate[k];
		estimate[k] += mc12->estimate[k];
		estimate[k] += mc13->estimate[k];
		estimate[k] += mc14->estimate[k];
		estimate[k] += mc15->estimate[k];
		estimate[k] += mc16->estimate[k];
		estimate[k] += mc17->estimate[k];
		estimate[k] += mc18->estimate[k];
	}

	clock_t lap2 = clock();
	std::cerr << "k";

	//順位をつける
	for (int k = 0; k < shotCount; k++) {
		for (int kk = 0; kk < shotCount; kk++) {
			if (estimate[k] < estimate[kk]) {
				better[k] ++;
			}
		}
	}

	std::cerr << "i";
	// 上位のいくつかに絞って繰り返し調べる。
	Montecarlo *mc21 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc22 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc23 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc24 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc25 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc26 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc27 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc28 = new Montecarlo(gs, shotCount, vec, weight, better);

	if (4 <= gs->ShotNum && gs->ShotNum < 12) {
		mc21->timeLimit = mc22->timeLimit = mc23->timeLimit = mc24->timeLimit = (clock_t)(remainingTimePart2 * 6 / 5);// 
		mc25->timeLimit = mc26->timeLimit = mc27->timeLimit = mc28->timeLimit = (clock_t)(remainingTimePart2 * 6 / 5);// 
	}
	else {
		mc21->timeLimit = mc22->timeLimit = mc23->timeLimit = mc24->timeLimit = (clock_t)(remainingTimePart2 * 4 / 5);// 
		mc25->timeLimit = mc26->timeLimit = mc27->timeLimit = mc28->timeLimit = (clock_t)(remainingTimePart2 * 4 / 5);// 
	}

	std::cerr << "n";
	thread thread21(bind(executeMontecarlo2, mc21));
	thread thread22(bind(executeMontecarlo2, mc22));
	thread thread23(bind(executeMontecarlo2, mc23));
	thread thread24(bind(executeMontecarlo2, mc24));
	thread thread25(bind(executeMontecarlo2, mc25));
	thread thread26(bind(executeMontecarlo2, mc26));
	thread thread27(bind(executeMontecarlo2, mc27));
	thread thread28(bind(executeMontecarlo2, mc28));

	thread21.join();
	thread22.join();
	thread23.join();
	thread24.join();
	thread25.join();
	thread26.join();
	thread27.join();
	thread28.join();

	std::cerr << "g";
	for (int k = 0; k < shotCount; k++) {
		if (better[k] < 5) {
			estimate[k] += mc21->estimate[k];
			estimate[k] += mc22->estimate[k];
			estimate[k] += mc23->estimate[k];
			estimate[k] += mc24->estimate[k];
			estimate[k] += mc25->estimate[k];
			estimate[k] += mc26->estimate[k];
			estimate[k] += mc27->estimate[k];
			estimate[k] += mc28->estimate[k];
		}
	}

	std::cerr << "." << endl;
	clock_t lap3 = clock();

	// 評価が一番高いショットを選択する。
	int maxK = 0;
	float max = -100000.0;
	for (int k = 0; k < shotCount; k++) {
		if (better[k] < 5 && max < estimate[k]) {
			max = estimate[k];
			maxK = k;
		}
	}

	// ショットを微調整する	

	int finalK = maxK;

	vec_ret->x = vec[finalK].x;
	vec_ret->y = vec[finalK].y;
	vec_ret->angle = vec[finalK].angle;

	std::cerr << " " << totalRepeatNumber1 << "+" << totalRepeatNumber2 <<  "(simulations)" << endl;
	std::cerr << " I tried " << shotName[finalK] << "." << endl;

	clock_t finish = clock();
	std::cerr << " " << (lap1 - start) << "+" << (lap2 - lap1) <<  "+" << (finish - lap2) << "(ms)" << endl;

	remainingTime -= (finish - start);
	std::cerr << "---------------------" << endl;

	// アイディアメモ
	// 対戦相手のショットの種類を推測する
	// ランダムショットの相手のプレー分はこの推測に基づくショットを投げるようにする。（精度をよくするため）
}


void getBestShot(const GAMESTATE* const gs, SHOTVEC *vec_ret) {

	myMT1(gs, vec_ret);
	//CreateSampleShot(gs, vec_ret);
}
