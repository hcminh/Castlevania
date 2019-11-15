#pragma once
#include "GameObject.h"
#include "debug.h"

enum BrickType {
	BGROUND
};

class CGround : public CGameObject
{
public:
	CGround(float x, float y, int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	
};