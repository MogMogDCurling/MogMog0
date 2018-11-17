// createShot.cpp


#include "createShot.h"
#include "strategy.h"
#include "utilities.h"

#include <random>

using namespace std;

//�ȈՔŃV���b�g���͂����ǂ����̔���
bool EasyCanShotReach(const GAMESTATE* const gs, SHOTPOS pos) {
	for (int stone = 0; stone < 16; stone++) {
		float xx = gs->body[stone][0];
		float yy = gs->body[stone][1];
		float incline_y = HOUSE_R * 1.5f;
		float incline_x = 2.0f * STONE_R;
		float dx = (TEE_X - pos.x) * incline_y / (41.28f - pos.y);
		if (xx > 0.0 && yy > 0.0) {
			if (pos.angle) {
				if (yy > pos.y && yy <= pos.y + incline_y)
					if ((incline_x + dx) / incline_y * (yy - pos.y) >= (xx - pos.x - incline_x))
						if ((incline_x + dx) / incline_y * (yy - pos.y) <= (xx - pos.x + incline_x)) {
							return false;
						}
			}
			else {
				if (yy >= pos.y && yy <= pos.y + incline_y)
					if (-(incline_x - dx) / incline_y * (yy - pos.y) >= (xx - pos.x - incline_x))
						if (-(incline_x - dx) / incline_y * (yy - pos.y) <= (xx - pos.x + incline_x)) {
							return false;
						}
			}
		}
	}
	return true;
}


// �e�B�[�ɒu���V���b�g
bool CreateTeeShot(const GAMESTATE* const gs, SHOTVEC *vec_ret) {
	SHOTPOS pos;
	// pos:center of House
	pos.x = TEE_X;
	pos.y = TEE_Y;
	pos.angle = SHOT_LEFT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	pos.angle = SHOT_RIGHT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	return false;
}

// �n�E�X�ɒu���ɍs���V���b�g
bool CreateDrawShot(const GAMESTATE* const gs, int position, SHOTVEC *vec_ret) {
	SHOTPOS pos;
	switch (position) {
	case 0:
		pos.x = TEE_X + 0.5f * HOUSE_R;
		pos.y = TEE_Y;
		break;
	case 1:
		pos.x = TEE_X - 0.5f * HOUSE_R;
		pos.y = TEE_Y;
		break;
	case 2:
		pos.x = TEE_X + 0.2f * HOUSE_R;
		pos.y = TEE_Y;
		break;
	case 3:
		pos.x = TEE_X - 0.2f * HOUSE_R;
		pos.y = TEE_Y;
		break;
	case 4:
		pos.x = TEE_X;
		pos.y = TEE_Y + 0.5f * HOUSE_R;
		break;
	case 5:
		pos.x = TEE_X;
		pos.y = TEE_Y - 0.5f * HOUSE_R;
		break;
	}
	pos.angle = SHOT_LEFT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	pos.angle = SHOT_RIGHT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	return false;
}

// �K�[�h��u���V���b�g
bool CreateGuardShot(const GAMESTATE* const gs, int position, SHOTVEC *vec_ret) {
	SHOTPOS pos;
	// �K�[�h�X�g�[���������V���b�g
	// 
	switch (position) {
	case CENTER_GUARD:
		pos.x = TEE_X;
		pos.y = 0.6f * (TEE_Y + HOUSE_R) + 0.4f * PLAYAREA_Y_MAX;
		break;
	case TIGHT_GUARD:
		pos.x = TEE_X;
		pos.y = 0.8f * (TEE_Y + HOUSE_R) + 0.2f * PLAYAREA_Y_MAX;
		break;
	case LEFT_GUARD:
		pos.x = TEE_X - HOUSE_R * 0.5f;
		pos.y = (3 * (TEE_Y + HOUSE_R) + PLAYAREA_Y_MAX) * 0.25;
		break;
	case RIGHT_GUARD:
	default:
		pos.x = TEE_X + HOUSE_R * 0.5f;
		pos.y = (3 * (TEE_Y + HOUSE_R) + PLAYAREA_Y_MAX) * 0.25;
		break;
	}
	pos.angle = SHOT_LEFT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	pos.angle = SHOT_RIGHT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	return false;
}

// �X�g�[�����K�[�h����΂�u���V���b�g
bool CreateStoneGuardShot(const GAMESTATE* const gs, int stone, SHOTVEC *vec_ret) {
	if (stone % 2 != gs->ShotNum % 2) return false;
	SHOTPOS pos;
	// �K�[�h�X�g�[���������V���b�g
	// 
	pos.x = gs->body[stone][0];
	pos.y = gs->body[stone][1] + 1.3f * HOUSE_R;
	pos.angle = SHOT_LEFT;
	CreateShot(pos, vec_ret);
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateShot(pos, vec_ret);
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//���ɉ񂷃V���b�g
bool CreateComearoundShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret) {
	// if not active, return false;
	if (gs->body[stone][0] == 0.0 || gs->body[stone][1] == 0.0) return false;
	// if not a guard stone, return false;
	if (gs->body[stone][1] <= TEE_Y + HOUSE_R) return false;

	SHOTPOS pos;
	pos.x = gs->body[stone][0];
	pos.y = gs->body[stone][1] - HOUSE_R * 1.5f;

	pos.angle = SHOT_LEFT;
	CreateShot(pos, vec_ret);
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateShot(pos, vec_ret);
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//�΂Ƀs�^���Ƃ���V���b�g
bool CreateFreezeShot(const GAMESTATE* const gs, int stone, int freezeAngle, SHOTVEC* vec_ret) {
	// ANGLE_ZERO : ����
	// ANGLE_LEFT : ��
	// ANGLE_RIGHT : �E
	SHOTPOS pos;
	switch (freezeAngle) {
	case ANGLE_ZERO:
		pos.x = gs->body[stone][0];
		pos.y = gs->body[stone][1] + 3.0f * STONE_R;
		break;
	case ANGLE_LEFT:
		pos.x = gs->body[stone][0] - STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	case ANGLE_RIGHT:
	default:
		pos.x = gs->body[stone][0] + STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	}
	pos.angle = SHOT_LEFT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	pos.angle = SHOT_RIGHT;
	if (EasyCanShotReach(gs, pos)) {
		CreateShot(pos, vec_ret);
		return true;
	}
	return false;
}

//�΂������V���b�g
bool CreatePromotionShot(const GAMESTATE* const gs, int stone, int PromotionAngle, SHOTVEC* vec_ret) {
	// ANGLE_ZERO : ����
	// ANGLE_LEFT : ��
	// ANGLE_RIGHT : �E
	SHOTPOS pos;
	switch (PromotionAngle) {
	case ANGLE_ZERO:
		pos.x = gs->body[stone][0];
		pos.y = gs->body[stone][1] + 3.0f * STONE_R;
		break;
	case ANGLE_LEFT:
		pos.x = gs->body[stone][0] - STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	case ANGLE_RIGHT:
	default:
		pos.x = gs->body[stone][0] + STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	}
	pos.angle = SHOT_LEFT;
	CreateHitShot(pos, 7, vec_ret);//Tee line�܂œ͂������B
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateHitShot(pos, 7, vec_ret);//Tee line�܂œ͂������B
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//�΂��͂����Ď����������V���b�g
bool CreateHitAndRollShot(const GAMESTATE* const gs, int stone, int HitAngle, SHOTVEC* vec_ret) {

	// ANGLE_ZERO : �q�b�g�A���h�X�e�C�V���b�g
	// ANGLE_LEFT : ��
	// ANGLE_RIGHT : �E
	SHOTPOS pos;
	switch (HitAngle) {
	case ANGLE_ZERO: case WEAK_ANGLE_ZERO:
		pos.x = gs->body[stone][0];
		pos.y = gs->body[stone][1] + 3.0f * STONE_R;
		break;
	case ANGLE_LEFT: case WEAK_ANGLE_LEFT:
		pos.x = gs->body[stone][0] - STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	case ANGLE_RIGHT: case WEAK_ANGLE_RIGHT:
	default:
		pos.x = gs->body[stone][0] + STONE_R;
		pos.y = gs->body[stone][1] + 2.7f * STONE_R;
		break;
	}
	pos.angle = SHOT_LEFT;
	switch (HitAngle) {
	case ANGLE_ZERO: case ANGLE_LEFT: case ANGLE_RIGHT:
		CreateHitShot(pos, 14, vec_ret);//�͂��������B
		break;
	case WEAK_ANGLE_ZERO: case WEAK_ANGLE_LEFT: case WEAK_ANGLE_RIGHT:
		CreateHitShot(pos, 11, vec_ret);//�͂��������B
		break;
	}
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	switch (HitAngle) {
	case ANGLE_ZERO: case ANGLE_LEFT: case ANGLE_RIGHT:
		CreateHitShot(pos, 14, vec_ret);//�͂��������B
		break;
	case WEAK_ANGLE_ZERO: case WEAK_ANGLE_LEFT: case WEAK_ANGLE_RIGHT:
		CreateHitShot(pos, 11, vec_ret);//�͂��������B
		break;
	}
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//�΂��͂����Ď����͂Ƃǂ܂�V���b�g
bool CreateHitAndStayShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret) {
	if (stone % 2 == gs->ShotNum % 2) return false;

	SHOTPOS pos;
	pos.x = gs->body[stone][0];
	pos.y = gs->body[stone][1] + 3.0f * STONE_R;

	pos.angle = SHOT_LEFT;
	CreateHitShot(pos, 14, vec_ret);//�͂��������B
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateHitShot(pos, 14, vec_ret);//�͂��������B
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

void CreateRandomShot(const GAMESTATE* const gs, SHOTVEC* vec_ret) {
	random_device rd;   // non-deterministic generator  
	mt19937 gen(rd());  // to seed mersenne twister.  

						//�y���������ς����Ă���T�C�R����U���āCvec���P������������΂悢�B
	int shotCount = 0;

	shotCount = 1; // teeshot 
	for (int stone = 0; stone < gs->ShotNum; stone++) {
		if (gs->body[stone][0] > 0.0 && gs->body[stone][1] > 0.0) {
			shotCount += 6;
		}
	}
	int k = (gen()) % shotCount;
	//std::cerr << k << "-th shot in random list." << endl;
	if (k < 1) {
		CreateTeeShot(gs, vec_ret);
		return;
	}
	k -= 1;
	for (int stone = 0; stone < gs->ShotNum; stone++) {
		if (gs->body[stone][0] > 0.0 && gs->body[stone][1] > 0.0) {
			if (k < 6) {
				switch (k) {
				case 0:case 1: case 2:
					CreateHitAndRollShot(gs, stone, k, vec_ret); break;
				case 3: case 4:	case 5:
					if (!CreateFreezeShot(gs, stone, k - 3, vec_ret)) {
						CreatePromotionShot(gs, stone, k - 3, vec_ret);
					}
					break;
				}
				return;
			}
			else {
				k -= 6;
			}
		}
	}
	//���������~�X������΁C�����ʂ��Ă��Ȃ��Ă�no1���q�b�g����B
	int rank[16];
	getRanking(rank, gs, gs->ShotNum);
	if (rank[0] != -1) {
		CreateHitAndStayShot(gs, rank[0], vec_ret);
	}
	else {
		CreateTeeShot(gs, vec_ret);
	}
}

// original heuristic algorithm
void CreateSampleShot(const GAMESTATE* const gs, SHOTVEC *vec_ret)
{
	random_device rd;   // non-deterministic generator  
	mt19937 gen(rd());  // to seed mersenne twister.  
	SHOTPOS pos;

	int rank[16];
	getRanking(rank, gs, gs->ShotNum);
	int itsme = gs->ShotNum % 2;

	if (rank[0] != -1) {
		// get position of the objective Stone
		pos.x = gs->body[rank[0]][0];
		pos.y = gs->body[rank[0]][1];

		if (rank[0] % 2 != itsme) {
			//Nr,1�X�g�[��������̐΂�������take out �V���b�g�����݂�B
			if (!CreateHitAndRollShot(gs, rank[0], gen() % 6, vec_ret)) {
				if (gs->ShotNum - 1 >= 0 && gs->ShotNum != 15) {
					//�����V���b�g���͂��Ȃ��悤�ł���΁A���O�̐΂��e�C�N�A�E�g���ɍs���B
					CreateHitAndRollShot(gs, gs->ShotNum - 1, gen() % 6, vec_ret);
				}
			}
		}
		else {
			//Nr,1�X�g�[���������̐΂�������K�[�h�V���b�g�����݂�B
			CreateStoneGuardShot(gs, rank[0], vec_ret);
		}
	}
	else {
		// �e�B�[�V���b�g�����݂�.
		CreateTeeShot(gs, vec_ret);
	}
}

