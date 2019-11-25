#pragma once
#include "GameObject.h"

#define ENEMY_WALKING_SPEED 0.05f;

#define ENEMY_BURN_TIME			1000	//CHAYS
#define ENEMY_RESPAWN_TIME		10000	//hoi sinh

#define ENEMY_STATE_WALKING 100
#define ENEMY_STATE_DIE 200

#define DOG_ANI_SIT_RIGHT			0	//chó NGỒI R
#define DOG_ANI_SIT_LEFT			1	//chó NGỒI L
#define DOG_ANI_WALK_RIGHT			2	//chó đi phải
#define DOG_ANI_WALK_LEFT			3	//chó đi trái
#define FISH_ANI_SHOOT_RIGHT		4	//cá bắn phải
#define FISH_ANI_SHOOT_LEFT			5	//cá bắn trái
#define FISH_ANI_IDLE_RIGHT			6	//cá đứng phải
#define FISH_ANI_IDLE_LEFT			7	//cá đứng trái
#define FISH_ANI_WALK_RIGHT			8	//cá đi phải
#define FISH_ANI_WALK_LEFT			9	//cá đi trái
#define BAT_ANI_IDLE				12	//dơi đậu
#define BAT_ANI_WALK_RIGHT			13	//dơi bay phải
#define BAT_ANI_WALK_LEFT			14	//dơi bay trái
#define SUPER_BAT_ANI_IDLE			15	//dơi trùm đậu
#define SUPER_BAT_ANI_FLY			16	//dơi trùm vỗ cánh


#define GHOST_BBOX_WIDTH 30
#define GHOST_BBOX_HEIGHT 65
#define DOG_BBOX_WIDTH 65
#define DOG_BBOX_HEIGHT 32
#define BAT_BBOX	 32
#define SUPER_BAT_BBOX_WIDTH 96
#define SUPER_BAT_BBOX_HEIGHT 44

enum EnemyType {
	GHOST,
	DOG,
	BAT,
	FISH,
	SUPPER_BAT,
};

class CEnemy : public CGameObject
{

public:

	DWORD burningStart;
	DWORD respawnTime;
	bool isRespawning = false;
	bool isBurning = false;
	bool isDead = true;
	int HP;
	int score;
	int damage;
	EnemyType enemyType;

	CEnemy();
	void startBurning() { isBurning = true; burningStart = GetTickCount(); }
	void startRespawnTime() { isRespawning = false; respawnTime = GetTickCount(); }
	virtual void respawn() = 0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
};