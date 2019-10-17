#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Whip.h"

#define ID_TEX_SIMON 0

#define SIMON_WALKING_SPEED		0.1f 
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT				500
#define SIMON_STATE_ATTACK			600

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1

#define SIMON_ANI_SIT_RIGHT				2
#define SIMON_ANI_SIT_LEFT				3

#define SIMON_ANI_WALKING_RIGHT			4
#define SIMON_ANI_WALKING_LEFT			5


#define SIMON_ANI_JUMP_RIGHT			6
#define SIMON_ANI_JUMP_LEFT				7

#define SIMON_ANI_DIE					8

#define SIMON_ANI_ATTACK_RIGHT			9
#define SIMON_ANI_ATTACK_LEFT			10

#define SIMON_ANI_SIT_ATTACK_RIGHT			11
#define SIMON_ANI_SIT_ATTACK_LEFT			12

#define MAX_ATTACK_FRAME				3

#define SIMON_SPRITE_WIDTH  60
#define SIMON_SPRITE_HEIGHT 66

#define PADDING 15

#define SIMON_BBOX_WIDTH  60
#define SIMON_BBOX_HEIGHT 63

#define SIMON_SIT_BBOX_WIDTH  60
#define SIMON_SIT_BBOX_HEIGHT 45

#define SIMON_STATE_STANDUP		30

#define SIMON_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
	static CSimon * __instance; // Singleton Patern

	int currentFrame = 0;
	int untouchable;
	DWORD untouchable_start;


public:
	//biến để kiểm tra Simon đang làm việc khác
	bool isJumping = false;
	bool isAttacking = false;
	bool isSitting = false;
	bool isMoving = false;

	CWhip *whip;


	CSimon() : CGameObject()
	{
		untouchable = 0;
	}

	static CSimon * GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void LoadResources();
	void Attack();
};