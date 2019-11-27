#pragma once

#include "GameObject.h"
#include <vector>

#define STAIR_BBOX_WIDTH			48
#define STAIR_BBOX_HEIGHT			32

enum CheckPoint {
	BOTTOM,
	TOP
};

class CStair : public CGameObject
{
public:
	CheckPoint checkPoint;
	CStair(CheckPoint cp);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

typedef CStair * LPSTAIR;


