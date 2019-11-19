#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Item.h"
#include "Weapon.h"

#define ID_TEX_SIMON 0

#define SIMON_WALKING_SPEED		 0.3f 	
#define SIMON_AUTO_GO_SPEED		0.02f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALKING_RIGHT		1
#define SIMON_STATE_WALKING_LEFT		2
#define SIMON_STATE_JUMP				3
#define SIMON_STATE_DIE					4
#define SIMON_STATE_SIT					5
#define SIMON_STATE_ATTACK				6
#define SIMON_STATE_ATTACK_WITH_SUB		7
#define SIMON_STATE_LEVEL_UP			8
#define SIMON_STATE_AUTO_GO				9
#define SIMON_STATE_ATTACKED			10
#define SIMON_STATE_STANDUP_AFTER_FALL	11


#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_SIT_RIGHT				2
#define SIMON_ANI_SIT_LEFT				3
#define SIMON_ANI_JUMP_RIGHT			4
#define SIMON_ANI_JUMP_LEFT				5
#define SIMON_ANI_WALKING_RIGHT			6
#define SIMON_ANI_WALKING_LEFT			7
#define SIMON_ANI_ATTACK_RIGHT			8
#define SIMON_ANI_ATTACK_LEFT			9
#define SIMON_ANI_SIT_ATTACK_RIGHT		10
#define SIMON_ANI_SIT_ATTACK_LEFT		11
#define SIMON_ANI_HURT_RIGHT			12
#define SIMON_ANI_HURT_LEFT				13
#define SIMON_ANI_DIE					16

#define MAX_ATTACK_FRAME				3

#define SIMON_SPRITE_WIDTH  60
#define SIMON_SPRITE_HEIGHT 66

#define PADDING 15

#define SIMON_BBOX_WIDTH  60
#define SIMON_BBOX_HEIGHT 63

#define SIMON_SIT_BBOX_WIDTH  60
#define SIMON_SIT_BBOX_HEIGHT 45

#define SIMON_STATE_STANDUP		30

#define SIMON_UNTOUCHABLE_TIME		 2000
#define SIMON_UP_LEVEL_TIME			 700
#define SIMON_HURTING_TIME			 200
#define SIMON_SIT_AFTER_FALL_TIME	 2000


class CSimon : public CGameObject
{
	static CSimon * __instance; // Singleton Patern

	DWORD accuTime = 0; //thời gian tích lũy được
	DWORD accuTimeStopWatch = 0; //thời gian tích lũy được
	DWORD untouchableStart;
	DWORD hurtingStart;
	DWORD levelUpStart;
	DWORD invisibleStart;
	DWORD sittingStart;
	float destinationX;
	int ani;
public:
	//biến để kiểm tra Simon đang làm việc khác
	bool isJumping = false;
	bool isAttacking = false;
	bool isHurting = false;
	bool isUseSubWeapon = false;
	bool isUsingStopWatch = false;
	bool isSitting = false;
	bool isMoving = false;
	bool levelUpgrade = false;
	bool isAutoGoX = false;
	bool isOnStair = false;
	bool untouchable = false;
	bool invisible = false;

	CWhip *whip;
	CWeapon *subWeapon = NULL;
	WeaponType typeSubWeapon = WeaponType::NONE;

	CSimon() : CGameObject()
	{
		LoadResources();
		SetPosition(0, 300.0f);
		whip = new CWhip(x, y, nx);
		subWeapon = new CWeapon();
		typeSubWeapon = WeaponType::NONE;
		subWeapon->SetState(typeSubWeapon);
	}

	static CSimon * GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isColisionItem(CItem *item);
	void colisionItem(CItem *item);
	void autoGotoX(float x);
	void attack();
	void startUntouchable() { untouchable = true; untouchableStart = GetTickCount(); }
	void startHurting() { isHurting = true; hurtingStart = GetTickCount(); }
	void startLevelUp() { levelUpgrade = true; levelUpStart = GetTickCount(); }
	void startInvisible() { invisible = true; invisibleStart = GetTickCount(); }
	void LoadResources();
};

typedef CSimon * LPSIMON;