#include <random>  
#include <string>
#include <ctime>

#include "strategy.h"
#include "createShot.h"
#include "utilities.h"
#include "estimate.h"


using namespace std;


const unsigned int shotVariations = 100;

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
			// �P���i�����j
			Simulation(gstmp, mc->vec[k], RANDOM, NULL, -1);
			mc->estimate[k] += makeEstimateOriginal(gstmp, shotNum, itsme, 1-itsme);// �]��
			shotNum++;
			if (shotNum < 16) {
				//��Q���F�����_���V���b�g�i����j
				//CreateRandomShot(gstmp, &vectmp);
				CreateSampleShot(gstmp, &vectmp);
				Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
				mc->estimate[k] += (NEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// �]��
				shotNum++;
			}
			if (shotNum < 16) {
				//��R���F�����_���V���b�g�i�����j
				//CreateSampleShot(gstmp, &vectmp);
				CreateRandomShot(gstmp, &vectmp);
				Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
				mc->estimate[k] += (NNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// �]��
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
				// �Ֆʂ̃R�s�[
				memcpy_s(gstmp, sizeof(GAMESTATE), mc->gs, sizeof(GAMESTATE));
				// �P���i�����j
				getRanking(thisRank, gstmp, shotNum);
				Simulation(gstmp, mc->vec[k], RANDOM, NULL, -1);
				shotNum++;
				mc->estimate[k] += makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme);// �]��
				if (shotNum < 16) {
					//��Q���F�����_���V���b�g�i����j
					//CreateRandomShot(gstmp, &vectmp);
					CreateSampleShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// �]��
				}
				if (shotNum < 16) {
					//��R���F�����_���V���b�g�i�����j
					CreateRandomShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// �]��
				}
				if (shotNum < 16) {
					//��S���F�����_���V���b�g�i����j
					//CreateRandomShot(gstmp, &vectmp);
					CreateSampleShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, OPP_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, itsme));// �]��
				}
				if (shotNum < 16) {
					//��T���F�����_���V���b�g�i�����j
					CreateRandomShot(gstmp, &vectmp);
					Simulation(gstmp, vectmp, MY_RANDOM, NULL, -1);
					shotNum++;
					mc->estimate[k] += (NNNNEXT_EST * makeEstimateOriginal(gstmp, shotNum, itsme, 1 - itsme));// �]��
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

//�����e�J�������x�[�X�Ƃ���mogmog�̎啔
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
		remainingTimePart1 = (clock_t)(remainingTime / 5 * 3 / remainingEnd / 8);
		remainingTimePart2 = (clock_t)(remainingTime / 5 * 2 / remainingEnd / 8);
		//remainingTimePart3 = (clock_t)(remainingTime / 5 / remainingEnd / 8);
	}


	int stoneInHouse = 0;
	for (int stone = 0; stone < 16; stone++) {
		if (isInHouse(gs->body[stone][0], gs->body[stone][1]) || TEE_Y + HOUSE_R < gs->body[stone][1]) {
			stoneInHouse++;
		}
	}
	if (stoneInHouse == 0 && gs->ShotNum < 14) {// -> �n�E�X���󂾂�����
		CreateGuardShot(gs, 1, vec_ret);
		clock_t finish = clock();
		std::cerr << " " << (double)(finish - start) << "(ms)" << endl;
		remainingTime -= (finish - start);
		std::cerr << "---------------------" << endl;
		return;
	}

	// 5�胂���e�J����
	// �\���̂����𗅗񂷂�Bvec[]�ɂ���B
	SHOTVEC vec[shotVariations];
	// �V���b�g�̓��e��\��string�ϐ� string shotName[] ���������Ă����Ƃ悢�����B
	string shotName[shotVariations];
	// ���̗���Ɠ��������̕]���l�̔z�����������B
	float weight[shotVariations];
	float estimate[shotVariations];
	int better[shotVariations];

	// �\���̂����𗅗񂷂�B
	int shotCount = 0;
	if (CreateTeeShot(gs, &(vec[shotCount]))) {
		shotName[shotCount] = "a tee shot ";
		weight[shotCount] = 0.0f;
		shotCount++;
	}
	for (int stone = gs->ShotNum - 1; stone >= 0; stone--) {
		if (gs->body[stone][0] > 0.0 && gs->body[stone][1] > 0.0) {
			for (int i = 0; i < 6; i++) {
				if (CreateHitAndRollShot(gs, stone, i, &(vec[shotCount]))) {
					// �V���b�g2�܂ł́C�q�b�g�A���h���[���͎��l�B
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
		if (CreateGuardShot(gs, i, &(vec[shotCount]))) {
			switch (i) {
			case 0:shotName[shotCount] = "a center guard shot"; break;
			case 1:shotName[shotCount] = "a tight guard shot"; break;
			}
			weight[shotCount] = 0.0f;
			shotCount++;
		}
	}
	int itsme = gs->ShotNum % 2;

	for (int k = 0; k < shotCount; k++) {
		estimate[k] = 0.0f;
		better[k] = 0;
	}

	clock_t lap1 = clock();

	Montecarlo *mc11 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc12 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc13 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc14 = new Montecarlo(gs, shotCount, vec, weight, better);

	mc11->timeLimit = mc12->timeLimit = mc13->timeLimit = mc14->timeLimit =
		(clock_t)(remainingTimePart1 * timeLimitPercent[gs->ShotNum] / 100);// 

	thread thread11(bind(executeMontecarlo1, mc11));
	thread thread12(bind(executeMontecarlo1, mc12));
	thread thread13(bind(executeMontecarlo1, mc13));
	thread thread14(bind(executeMontecarlo1, mc14));

	thread11.join();
	thread12.join();
	thread13.join();
	thread14.join();


	for (int k = 0; k < shotCount; k++) {
		estimate[k] = mc11->estimate[k];
		estimate[k] += mc12->estimate[k];
		estimate[k] += mc13->estimate[k];
		estimate[k] += mc14->estimate[k];
	}

	clock_t lap2 = clock();

	//���ʂ�����
	for (int k = 0; k < shotCount; k++) {
		for (int kk = 0; kk < shotCount; kk++) {
			if (estimate[k] < estimate[kk]) {
				better[k] ++;
			}
		}
	}

	// ��ʂ̂������ɍi���ČJ��Ԃ����ׂ�B
	Montecarlo *mc21 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc22 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc23 = new Montecarlo(gs, shotCount, vec, weight, better);
	Montecarlo *mc24 = new Montecarlo(gs, shotCount, vec, weight, better);

	if (4 <= gs->ShotNum && gs->ShotNum < 12) {
		mc21->timeLimit = mc22->timeLimit = mc23->timeLimit = mc24->timeLimit = (clock_t)(remainingTimePart2 * 6 / 5);// 
	}
	else {
		mc21->timeLimit = mc22->timeLimit = mc23->timeLimit = mc24->timeLimit = (clock_t)(remainingTimePart2 * 4 / 5);// 
	}

	thread thread21(bind(executeMontecarlo2, mc21));
	thread thread22(bind(executeMontecarlo2, mc22));
	thread thread23(bind(executeMontecarlo2, mc23));
	thread thread24(bind(executeMontecarlo2, mc24));

	thread21.join();
	thread22.join();
	thread23.join();
	thread24.join();

	for (int k = 0; k < shotCount; k++) {
		if (better[k] < 5) {
			estimate[k] = mc21->estimate[k];
			estimate[k] += mc22->estimate[k];
			estimate[k] += mc23->estimate[k];
			estimate[k] += mc24->estimate[k];
		}
	}

	clock_t lap3 = clock();

	// �]������ԍ����V���b�g��I������B
	int maxK = 0;
	float max = -100000.0;
	for (int k = 0; k < shotCount; k++) {
		if (better[k] < 5 && max < estimate[k]) {
			max = estimate[k];
			maxK = k;
		}
	}

	// �V���b�g�����������	

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

	// �A�C�f�B�A����
	// �ΐ푊��̃V���b�g�̎�ނ𐄑�����
	// �����_���V���b�g�̑���̃v���[���͂��̐����Ɋ�Â��V���b�g�𓊂���悤�ɂ���B�i���x���悭���邽�߁j
}


void getBestShot(const GAMESTATE* const gs, SHOTVEC *vec_ret) {

	myMT1(gs, vec_ret);
	//CreateSampleShot(gs, vec_ret);
}