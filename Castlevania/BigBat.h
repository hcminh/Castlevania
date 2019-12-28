#pragma once

#include "Enemy.h"


// Boss
#define BIG_BAT_BBOX_WIDTH						100
#define BIG_BAT_BBOX_HEIGHT						46
#define BIG_BAT_RECT_RANDOMSPOT_BBOX_WIDTH		200
#define BIG_BAT_RECT_RANDOMSPOT_BBOX_HEIGHT		200

// Boss
#define BIG_BAT_DEFAULT_TIME_TO_FLY		1000
#define BIG_BAT_FAST_TIME_TO_FLY		1000
#define BIG_BAT_STOP_TIME_WAITING		1500
#define BIG_BAT_ATTACK_TIME_WAITING		3000


#define BIG_BAT_ANI_IDLE			0
#define BIG_BAT_ANI_FLYING			1
#define BIG_BAT_ANI_BURNING			2

// Boss 
#define BIG_BAT_STATE_IDLE				0
#define BIG_BAT_STATE_FLYING			1
#define BIG_BAT_STATE_BURNING			2
#define BIG_BAT_STATE_ACTIVE			3


class CBigBat : public CEnemy
{
	bool isFlyToTarget = false;
	bool isFlyToSimon = false;

	int xCam;

	int flyToSimonPercent;

	D3DXVECTOR2 simonPostion;
	D3DXVECTOR2 newPosition;

	int startTimeWaiting = 0;
	bool isStopWaiting = false;

	bool dropItem = false;
	bool isFlying = false;

	DWORD attackTime;;

public:
	CBigBat(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void respawn() {};

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void startAttack() { attackTime = GetTickCount(); }
	void FlyToTarget(DWORD dt, D3DXVECTOR2 targe);
	void Flying(DWORD dt);
	void randomNewPosition();

};


