#pragma once

#include "GameObject.h"

#define STAIR_BBOX_WIDTH			56
#define STAIR_BBOX_HEIGHT			16

#define LAST_STEP_HEIGHT		12.0f
#define LAST_STEP_WIDTH			15.0f
#define STEP_UP_HEIGHT			16.0f
#define STEP_UP_WIDTH			15.0f
#define STEP_DOWN_HEIGHT		15.5f
#define STEP_DOWN_WIDTH			15.5f


enum STATESTAIR {
	STAIR_NONE,
	UP_RIGHT,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT,
};

class CStair : public CGameObject
{
public:
	STATESTAIR stateStair = STATESTAIR::STAIR_NONE;
	int stairDirection;
	int firstLadderPosX = 0;
	CStair(STATESTAIR st, int direction, int firstLadderPosX, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

typedef CStair * LPSTAIR;


