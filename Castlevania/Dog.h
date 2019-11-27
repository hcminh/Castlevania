#pragma once

#include "Enemy.h"

// Dog
#define DOG_BBOX_WIDTH		65
#define DOG_BBOX_HEIGHT		32

#define DOG_WALKING_SPEED		0.17f
#define DOG_JUMPING_SPEED		0.5f
#define DOG_GRAVITY				0.002f

#define DOG_STATE_DEAD			0
#define DOG_STATE_RESPAWN		1
#define DOG_STATE_JUMP			2

#define DOG_ANI_SIT_RIGHT			0	//chó NGỒI R
#define DOG_ANI_SIT_LEFT			1	//chó NGỒI L
#define DOG_ANI_WALK_RIGHT			2	//chó đi phải
#define DOG_ANI_WALK_LEFT			3	//chó đi trái
#define DOG_ANI_JUMP_RIGHT			4	//chó nhảy phải
#define DOG_ANI_JUMP_LEFT			5	//chó nhảy trái
#define DOG_ANI_BURNING				6	//chó chết
class CDog : public CEnemy
{
public:
	bool isJumping = false;
	bool isSitting = false;
	CDog();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void respawn();
	virtual void dead();
	virtual void active();
	virtual void SetState(int state);
};

