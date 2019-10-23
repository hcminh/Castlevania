#pragma once
#include "GameObject.h"
#include "debug.h"

#define DOOR_BBOX_WIDTH  32
#define DOOR_BBOX_HEIGHT 60

class CDoor : public CGameObject
{
public:
	CDoor(float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};