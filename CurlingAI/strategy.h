#pragma once

#include "main.h"
#include <thread>

/*** constant numbers ***/
static const float RANDOM = (float)0.145;  // sigma of normal random number

//! positions on sheet
static const float TEE_X = (float)2.375;    // x of center of house
static const float TEE_Y = (float)4.880;    // y of center of house
static const float HOUSE_R = (float)1.830;  // radius of house
static const float STONE_R = (float)0.145;  // radius of stone

//! coodinate (x, y) is in playarea IF:
//   (PLAYAREA_X_MIN < x < PLAYAREA_X_MAX && PLAYAREA_Y_MIN < y < PLAYAREA_Y_MAX)
static const float PLAYAREA_X_MIN = (float)0.000 + STONE_R;
static const float PLAYAREA_X_MAX = (float)0.000 + (float)4.750 - STONE_R;
static const float PLAYAREA_Y_MIN = (float)3.050 + STONE_R;
static const float PLAYAREA_Y_MAX = (float)3.050 + (float)8.230 + STONE_R;

//! choose best shot
void getBestShot(const GAMESTATE* const gs, SHOTVEC *vec_ret);

#define NEXT_EST 1.5f
#define NNEXT_EST 2.25f
#define NNNEXT_EST 3.375f
#define NNNNEXT_EST 5.0625f

#define COUNT_IN_HOUSE 1.0f

#define SHOT_LEFT false
#define SHOT_RIGHT true
#define CENTER_GUARD 0
#define TIGHT_GUARD 1
#define LEFT_GUARD 2
#define RIGHT_GUARD 3
#define ANGLE_ZERO 0
#define ANGLE_LEFT 1
#define ANGLE_RIGHT 2
#define WEAK_ANGLE_ZERO 3
#define WEAK_ANGLE_LEFT 4
#define WEAK_ANGLE_RIGHT 5

#define DEFENSIVE_WEIGHT 0.0f
#define OFFENSIVE_WEIGHT 0.0f

#define MY_RANDOM 0.145f
#define OPP_RANDOM 0.0f

