#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"
#include "Item.h"
//#include "Scenes.h"

// ID
#define ID_TEX_WEAPON			2

#define KNIFE_BBOX_WIDTH		35
#define KNIFE_BBOX_HEIGHT		15
#define AXE_BBOX				35

#define KNIFE_FLYING_SPEED		0.5f
#define AXE_SPEED_Y				0.5f
#define AXE_SPEED_X				0.17f

#define AXE_GRAVITY			0.001f

#define KNIFE_ANI_RIGHT			0
#define KNIFE_ANI_LEFT			1
#define AXE_ANI_RIGHT			2
#define AXE_ANI_LEFT			3

enum WeaponType {
	KNIFE_WEAPON,
	AXE_WEAPON
};

class CWeapon : public CGameObject
{
	int ani;
public:
	bool isFlying = false;
	CWeapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void SetState(int state);
	void setPosition(float x, float y);
	bool isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void LoadResources();
};

