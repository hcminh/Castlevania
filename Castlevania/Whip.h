#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Game.h"
#include "Candle.h"
//#include "Scenes.h"

// Whip
#define ID_TEX_WHIP		1

#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2

#define LONG_CHAIN_BBOX_WIDTH		85
#define WHIP_BBOX_WIDTH				55
#define RANGE_OF_LONG_NORMAL		30

#define WHIP_BBOX_HEIGHT			15


#define LEVEL_MAX			3
#define LEVEL_MIN			1

#define WHIP_HEIGHT			68
#define WHIP_WIDTH			160

#define RANGE_OF_WHIP_SIMON_X			40

class CWhip : public CGameObject
{
	int level = 1;
	int ani;
public:
	CWhip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void levelUp() { level += (level == LEVEL_MAX) ? 0 : 1; };
	void levelDown() { level -= (level == LEVEL_MIN) ? 0 : 1; };
	void setPosition(float x, float y);
	bool isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void LoadResources();
};

