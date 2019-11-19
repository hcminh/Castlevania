#pragma once

#include "Enemy.h"

// Zombie
#define ZOMBIE_BBOX_WIDTH			30
#define ZOMBIE_BBOX_HEIGHT			65

#define ZOMBIE_WALKING_SPEED		0.1f
#define ZOMBIE_GRAVITY				0.002f

#define ZOMBIE_ANI_WALK_RIGHT		0	//ma đi phải
#define ZOMBIE_ANI_WALK_LEFT		1	//ma đi trái

class CZombie : public CEnemy
{
public:


	CZombie();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

