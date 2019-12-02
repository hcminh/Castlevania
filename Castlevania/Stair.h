#pragma once

#include "GameObject.h"

#define STAIR_BBOX_WIDTH			48
#define STAIR_BBOX_HEIGHT			16

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
	int ladders = 0;
	int firstLadderPosX = 0;
	CStair(STATESTAIR st, int ladders, int firstLadderPosX, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

typedef CStair * LPSTAIR;


