#pragma once

#include "GameObject.h"

class CWater : public CGameObject
{

public:
	CWater();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

