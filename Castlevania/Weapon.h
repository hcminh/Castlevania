#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"
#include "Item.h"

// ID
#define ID_TEX_WEAPON			2

#define KNIFE_BBOX_WIDTH		35
#define KNIFE_BBOX_HEIGHT		15
#define AXE_BBOX				35
#define HOLY_WATER_BBOX			20
#define FIRER_BBOX				30

#define KNIFE_FLYING_SPEED		0.5f
#define AXE_SPEED_Y				0.5f
#define AXE_SPEED_X				0.17f
#define HOLY_WATER_SPEED_Y		0.25f
#define HOLY_WATER_SPEED_X		0.3f

#define AXE_GRAVITY				0.001f
#define HOLY_WATER_GRAVITY		0.0015f

#define FIRER_TIME_DISPLAY		2000

#define KNIFE_ANI_RIGHT			0
#define KNIFE_ANI_LEFT			1
#define AXE_ANI_RIGHT			2
#define AXE_ANI_LEFT			3
#define HOLY_WATER_ANI_RIGHT	4
#define HOLY_WATER_ANI_LEFT		5
#define FIRE_ANI				6


enum WeaponType {
	NONE,
	KNIFE_WEAPON,
	AXE_WEAPON,
	HOLY_WATER_WEAPON,
	STOP_WATCH_WEAPON,
	FIRER
};

class CWeapon : public CGameObject
{
	int ani;
	DWORD accutime = 0;
public:
	bool isFlying  = false;
	bool isBurning = false;
	bool isStopWatch = false;
	CWeapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetState(int state);
	bool isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void LoadResources();
};

