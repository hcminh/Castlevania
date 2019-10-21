#pragma once
#include "GameObject.h"
#include "debug.h"

#define GROUND_BBOX		32

class CGround : public CGameObject
{
public:
	CGround();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void LoadResources();
	
};