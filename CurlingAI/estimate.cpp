// estimate.cpp

#include "estimate.h"
#include "strategy.h"
#include "utilities.h"

using namespace std;



/**
gstmp : �Q�[���X�e�[�W
shotNum : ������V���b�g����̂��扽����0,1,...,15
itsme : �����̐΂� 0 �� 1 ���B
�@�@�@itsme���񂪗L���Ȃ�΃v���X�A�s���Ȃ�΃}�C�i�X�̕]���ɂȂ�B
shotPlayer : ���̃V���b�g��0�̐l��1�̂ЂƂ�
*/
float makeEstimateOriginal(const GAMESTATE* const gstmp, int shotNum, int itsme, int shotPlayer) {
	int rank[16];
	getRanking(rank, gstmp);
	int pt = getPoint(rank, itsme);
	return 1.0f*pt;
}


