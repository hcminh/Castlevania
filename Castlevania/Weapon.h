#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"
#include "Candle.h"
//#include "Scenes.h"

// ID
#define ID_TEX_SUB_WEAPON		50

#define KNIFE_BBOX_WIDTH		55
#define KNIFE_BBOX_HEIGHT		20

#define KNIFE_HEIGHT			68
#define KNIFE_WIDTH				160

#define KNIFE_FLYING_SPEED		0.4f

#define KNIFE_ANI_RIGHT			0
#define KNIFE_ANI_LEFT			1

enum WeaponType {
	KNIFE_WEAPON
};

class CWeapon : public CGameObject
{
	int direct = -1; //derect = 0 simon quay phai, direct = 1 simon quay trai
	int ani;
public:
	bool isFlying = false;
	CWeapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void setPosition(float x, float y, int direct);
	bool isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void LoadResources();
};

