#pragma once

#include "Enemy.h"

// Zombie
#define FIRE_BALL_BBOX_WIDTH			30
#define FIRE_BALL_BBOX_HEIGHT			65

#define FIRE_BALL_WALKING_SPEED		0.1f
#define FIRE_BALL_GRAVITY				0.002f

#define FIRE_BALL_ANI_DEAD	2

#define FIRE_BALL_STATE_SHOOT 0

class CFireBall : public CEnemy
{
public:
	bool isFlying;
	CFireBall(int nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void respawn();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

