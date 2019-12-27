#pragma once
#include "GameObject.h"
#include "debug.h"

#define STATE_NOT_COLLISION_WIDTH_ENEMY 1

enum BrickType {
	BGROUND
};

class CGround : public CGameObject
{
public:
	CGround(int state, int width, int height, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	
};