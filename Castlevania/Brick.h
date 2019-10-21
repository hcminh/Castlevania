#pragma once
#include "GameObject.h"
#include "debug.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
public:
	CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};