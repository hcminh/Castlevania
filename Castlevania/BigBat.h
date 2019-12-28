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


#define BIG_BAT_ANI_IDLE			0
#define BIG_BAT_ANI_FLYING			1
#define BIG_BAT_ANI_BURNING			2

// Boss 
#define BIG_BAT_STATE_IDLE				0
#define BIG_BAT_STATE_FLYING			1
#define BIG_BAT_STATE_BURNING			2


class CBigBat : public CEnemy
{
	bool isFlyToTarget = false;
	bool isFlyToSimon = false;

	int idTarget = 0;

	D3DXVECTOR2 simonPostion;
	D3DXVECTOR2 target;

	int startTimeWaiting = 0;
	bool isStopWaiting = false;

	bool dropItem = false;
	bool isFlying = false;

public:
	CBigBat(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);
	virtual void respawn() {};

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	D3DXVECTOR2 GetRandomSpot();
	void FlyToTarget(DWORD dt);
	void GetVelocity();

	void StartStopTimeCounter() { isStopWaiting = true; startTimeWaiting = GetTickCount(); }

	void SetSimonPosition(float sx, float sy) { simonPostion.x = sx; simonPostion.y = sy; }

	int GetIdTarget() { return idTarget; }

	bool DropItem() { return dropItem; }

};


