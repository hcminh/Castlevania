#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Item.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Stair.h"

#define ID_TEX_SIMON 0

#define SIMON_WALKING_SPEED		 0.15f 	
#define SIMON_AUTO_WALK_SPEED	 0.02f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f
#define SIMON_IS_DOWN_STAIR		 -1
#define SIMON_IS_UP_STAIR		  1
#define SIMON_IS_NOT_ON_STAIR	  0

#define SIMON_STAIR_SPEED_X			0.079f
#define SIMON_STAIR_SPEED_Y			0.079f
#define SIMON_SPEED_Y_LOWER_ZONE    0.2f
#define SIMON_GRAVITY_LOWER			0.001f

#define SIMON_STATE_IDLE				0
#define SIMON_STATE_WALKING_RIGHT		1
#define SIMON_STATE_WALKING_LEFT		2
#define SIMON_STATE_JUMP				3
#define SIMON_STATE_DIE					4
#define SIMON_STATE_SIT					5
#define SIMON_STATE_ATTACK				6
#define SIMON_STATE_ATTACK_WITH_SUB		7
#define SIMON_STATE_LEVEL_UP			8
#define SIMON_STATE_AUTO_WALK			9
#define SIMON_STATE_ATTACKED			10
#define SIMON_STATE_SIT_AFTER_FALL		11
#define SIMON_STATE_INVISIBLE			12
#define SIMON_STATE_UP_STAIR			13
#define SIMON_STATE_DOWN_STAIR			14
#define SIMON_STATE_IDLE_STAIR			15


#define ANI_IDLE_RIGHT				0
#define ANI_IDLE_LEFT				1
#define ANI_SIT_RIGHT				2
#define ANI_SIT_LEFT				3
#define ANI_JUMP_RIGHT				4
#define ANI_JUMP_LEFT				5
#define ANI_WALKING_RIGHT			6
#define ANI_WALKING_LEFT			7
#define ANI_ATTACK_RIGHT			8
#define ANI_ATTACK_LEFT				9
#define ANI_SIT_ATTACK_RIGHT		10
#define ANI_SIT_ATTACK_LEFT			11
#define ANI_HURT_RIGHT				12
#define ANI_HURT_LEFT				13
#define ANI_IDLE_UP_STAIR_RIGHT		14		// Đứng lên Phải	 	
#define ANI_IDLE_UP_STAIR_LEFT		15		// Đứng lên Trái	
#define ANI_UP_STAIR_RIGHT			16		// Đi lên Phải	
#define ANI_UP_STAIR_LEFT			17		// Đi lên Trái	
#define ANI_HIT_UP_STAIR_RIGHT		18		// Đánh lên phải	
#define ANI_HIT_UP_STAIR_LEFT		19		// Đánh lên trái	
#define ANI_IDLE_DOWN_STAIR_RIGHT	20		// Đứng Xuống Phải	
#define ANI_IDLE_DOWN_STAIR_LEFT	21		// Đứng Xuống Trái	
#define ANI_DOWN_STAIR_RIGHT		22		// Đi Xuống Phải	
#define ANI_DOWN_STAIR_LEFT			23		// Đi Xuống Trái	
#define ANI_HIT_DOWN_STAIR_RIGHT	24		// Đánh Xuống phải	
#define ANI_HIT_DOWN_STAIR_LEFT		25		// Đánh Xuống trái	
#define ANI_DIE_RIGHT				26
#define ANI_DIE_LEFT				27

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
#define SIMON_INVISIABLE_TIME		 5000
#define SIMON_UP_LEVEL_TIME			 700
#define SIMON_HURTING_TIME			 200
#define SIMON_SIT_AFTER_FALL_TIME	 400


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
	bool levelUpgrade = false;
	bool untouchable = false;
	bool invisible = false;
	bool isFalling = false;
	bool cantHandle = false;
	//stair
	bool isStartOnStair = false; //bắt đầu lên/xuống thang, biến này dùng để kiếm tra giúp cho hàm stairOnStair chỉ chạy 1 lần duy nhất khi bấm phím
	bool isOnStair = false;	// trạng thái đang đứng trên cầu thang 
	bool isMoving = false;
	bool isUpStair = false;
	bool isDownStair = false;

	// auto-walk
	bool isAutoWalk = false;		// tự động đi
	float autoDistance = 0;		// Khoảng cách 

	CWhip *whip;
	CWeapon *subWeapon = NULL;
	WeaponType typeSubWeapon = WeaponType::NONE;

	CSimon() : CGameObject()
	{
		LoadResources();
		type = ObjectType::SIMON;
		SetPosition(0, 300.0f);
		whip = new CWhip(x, y, nx);
		subWeapon = new CWeapon();
		typeSubWeapon = WeaponType::NONE;
		subWeapon->SetState(typeSubWeapon);
	}

	static CSimon * GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void updateState();
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isColisionItem(CItem *item);
	void colisionItem(CItem *item);
	bool isColisionEnemy(CEnemy *enemy);
	void colisionEnemy(CEnemy *enemy);
	bool isColisionWeapon(CWeapon *weapon);
	void colisionWeapon(CWeapon *weapon);
	void collisionSupporter(LPGAMEOBJECT obj);
	void upStair(vector<LPGAMEOBJECT> stairs);
	bool downStair(vector<LPGAMEOBJECT> stairs);
	void startOnStair(LPGAMEOBJECT stair);
	void attack();
	void autoWalk(float distance);
	void autoWalkToX(float x);
	void startUntouchable() { untouchable = true; untouchableStart = GetTickCount(); }
	void startHurting() { isHurting = true; hurtingStart = GetTickCount(); }
	void startLevelUp() { levelUpgrade = true; levelUpStart = GetTickCount(); }
	void startInvisible() { invisible = true; invisibleStart = GetTickCount(); }
	void startSittingAfterFall() { isSitting = true; isFalling = true; sittingStart = GetTickCount(); }
	void LoadResources();
};

typedef CSimon * LPSIMON;