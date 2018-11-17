#pragma once

// createShot.h

#include "CurlingSimulator.h"

// FullRandom 0.145f,0.145f
// HalfRandom 0.145f,0.0f
// NullRandom 0.0f,0.0f

bool EasyCanShotReach(const GAMESTATE* const gs, SHOTPOS pos);
bool CreateTeeShot(const GAMESTATE* const gs, SHOTVEC *vec_ret);
bool CreateDrawShot(const GAMESTATE* const gs, int position, SHOTVEC *vec_ret);
bool CreateGuardShot(const GAMESTATE* const gs, int position, SHOTVEC *vec_ret);
bool CreateStoneGuardShot(const GAMESTATE* const gs, int stone, SHOTVEC *vec_ret);
bool CreateComearoundShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret);
bool CreateFreezeShot(const GAMESTATE* const gs, int stone, int freezeAngle, SHOTVEC* vec_ret);
bool CreatePromotionShot(const GAMESTATE* const gs, int stone, int PromotionAngle, SHOTVEC* vec_ret);
bool CreateHitAndRollShot(const GAMESTATE* const gs, int stone, int HitAngle, SHOTVEC* vec_ret);
bool CreateHitAndStayShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret);
void CreateDoubleRollInShot(const GAMESTATE* const gs, int stone, SHOTVEC* vec_ret);
void CreateRandomShot(const GAMESTATE* const gs, SHOTVEC* vec_ret);
void CreateSampleShot(const GAMESTATE* const gs, SHOTVEC *vec_ret);
