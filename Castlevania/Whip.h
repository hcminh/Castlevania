#pragma once

#include "GameObject.h"
#include "debug.h"

#include "Simon.h"
#include "Game.h"

// Whip
#define ID_TEX_WHIP		5

#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2
#define LONG_CHAIN_BBOX_WIDTH		85
#define WHIP_BBOX_WIDTH				55
#define WHIP_BBOX_HEIGHT			15

#define WHIP_HEIGHT			66
#define WHIP_WIDTH			240	


class Whip : public CGameObject
{

public:
	Whip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMovement = false);
	virtual void Render();

	void Render(int currentID = -1);

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void setPosition();
	void LoadResources();
};

