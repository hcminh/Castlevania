#pragma once

#include "GameObject.h"

#define STAIR_BBOX_WIDTH			32
#define STAIR_BBOX_HEIGHT			32

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
	CStair(float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

typedef CStair * LPSTAIR;


