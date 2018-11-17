// createShot.cpp


#include "createShot.h"
#include "strategy.h"
#include "utilities.h"

#include <random>

using namespace std;

//簡易版ショットが届くかどうかの判定
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


// ティーに置くショット
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

// ハウスに置きに行くショット
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

// ガードを置くショット
bool CreateGuardShot(const GAMESTATE* const gs, int position, SHOTVEC *vec_ret) {
	SHOTPOS pos;
	// ガードストーンをおくショット
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

// ストーンをガードする石を置くショット
bool CreateStoneGuardShot(const GAMESTATE* const gs, int stone, SHOTVEC *vec_ret) {
	if (stone % 2 != gs->ShotNum % 2) return false;
	SHOTPOS pos;
	// ガードストーンをおくショット
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

//裏に回すショット
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

//石にピタリとつけるショット
bool CreateFreezeShot(const GAMESTATE* const gs, int stone, int freezeAngle, SHOTVEC* vec_ret) {
	// ANGLE_ZERO : 正面
	// ANGLE_LEFT : 左
	// ANGLE_RIGHT : 右
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

//石を押すショット
bool CreatePromotionShot(const GAMESTATE* const gs, int stone, int PromotionAngle, SHOTVEC* vec_ret) {
	// ANGLE_ZERO : 正面
	// ANGLE_LEFT : 左
	// ANGLE_RIGHT : 右
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
	CreateHitShot(pos, 7, vec_ret);//Tee lineまで届く強さ。
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateHitShot(pos, 7, vec_ret);//Tee lineまで届く強さ。
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//石をはじいて自分も動くショット
bool CreateHitAndRollShot(const GAMESTATE* const gs, int stone, int HitAngle, SHOTVEC* vec_ret) {

	// ANGLE_ZERO : ヒットアンドステイショット
	// ANGLE_LEFT : 左
	// ANGLE_RIGHT : 右
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
		CreateHitShot(pos, 14, vec_ret);//はじく強さ。
		break;
	case WEAK_ANGLE_ZERO: case WEAK_ANGLE_LEFT: case WEAK_ANGLE_RIGHT:
		CreateHitShot(pos, 11, vec_ret);//はじく強さ。
		break;
	}
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	switch (HitAngle) {
	case ANGLE_ZERO: case ANGLE_LEFT: case ANGLE_RIGHT:
		CreateHitShot(pos, 14, vec_ret);//はじく強さ。
		break;
	case WEAK_ANGLE_ZERO: case WEAK_ANGLE_LEFT: case WEAK_ANGLE_RIGHT:
		CreateHitShot(pos, 11, vec_ret);//はじく強さ。
		break;
	}
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

//石をはじいて自分はとどまるショット
bool CreateHitAndStayShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret) {
	if (stone % 2 == gs->ShotNum % 2) return false;

	SHOTPOS pos;
	pos.x = gs->body[stone][0];
	pos.y = gs->body[stone][1] + 3.0f * STONE_R;

	pos.angle = SHOT_LEFT;
	CreateHitShot(pos, 14, vec_ret);//はじく強さ。
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	pos.angle = SHOT_RIGHT;
	CreateHitShot(pos, 14, vec_ret);//はじく強さ。
	if (EasyCanShotReach(gs, pos)) {
		return true;
	}
	return false;
}

void CreateRandomShot(const GAMESTATE* const gs, SHOTVEC* vec_ret) {
	random_device rd;   // non-deterministic generator  
	mt19937 gen(rd());  // to seed mersenne twister.  

						//軽く個数を見積もってからサイコロを振って，vecを１つだけ生成すればよい。
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
	//もし処理ミスがあれば，道が通っていなくてもno1をヒットする。
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
			//Nr,1ストーンが相手の石だったらtake out ショットを試みる。
			if (!CreateHitAndRollShot(gs, rank[0], gen() % 6, vec_ret)) {
				if (gs->ShotNum - 1 >= 0 && gs->ShotNum != 15) {
					//もしショットが届かないようであれば、直前の石をテイクアウトしに行く。
					CreateHitAndRollShot(gs, gs->ShotNum - 1, gen() % 6, vec_ret);
				}
			}
		}
		else {
			//Nr,1ストーンが自分の石だったらガードショットを試みる。
			CreateStoneGuardShot(gs, rank[0], vec_ret);
		}
	}
	else {
		// ティーショットを試みる.
		CreateTeeShot(gs, vec_ret);
	}
}

