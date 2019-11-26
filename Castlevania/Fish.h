#pragma once
#include "Enemy.h"
#include "Weapon.h"

// Zombie
#define FISH_BBOX_WIDTH			30
#define FISH_BBOX_HEIGHT			65

#define FISH_WALKING_SPEED		0.1f
#define FISH_GRAVITY				0.002f

#define FISH_WAIT_TO_SHOOT_TIME		6000
#define FISH_SHOOTING_TIME			700

#define FISH_STATE_DEAD			0
#define FISH_STATE_RESPAWN		1
#define FISH_STATE_SHOOT		2

#define FISH_ANI_SHOOT_RIGHT		0	//cá bắn phải
#define FISH_ANI_SHOOT_LEFT			1	//cá bắn trái
#define FISH_ANI_IDLE_RIGHT			2	//cá đứng phải
#define FISH_ANI_IDLE_LEFT			3	//cá đứng trái
#define FISH_ANI_WALK_RIGHT			4	//cá đi phải
#define FISH_ANI_WALK_LEFT			5	//cá đi trái
#define FISH_ANI_DEAD				6	//cá chết

class CFish : public CEnemy
{
public:

	CWeapon *weapon = NULL;
	int waitToShoot = 0;
	DWORD shootingTime;
	bool isShooting = false;
	bool isFlying = false;
	bool isWalking = false;

	CFish();

	void startShoot() { isShooting = true; shootingTime = GetTickCount(); }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void respawn();
	virtual void dead();
	virtual void active();
	virtual void SetState(int state);
};

