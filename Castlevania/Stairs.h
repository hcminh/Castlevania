#pragma once

#include "GameObject.h"
#include <vector>

#define STAIR_BBOX_WIDTH			48
#define STAIR_BBOX_HEIGHT			16

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


class CStairs
{
	static CStairs * _instance;

public:

	vector< LPSTAIR> stairs;

	void Render();
	static CStairs * GetInstance();
};


