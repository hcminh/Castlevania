#pragma once
#include "GameObject.h"

#define ENEMY_WALKING_SPEED 0.1f;

#define ENEMY_BURN_TIME			500	//CHAYS
#define ENEMY_RESPAWN_TIME		3000	//hoi sinh

#define ENEMY_STATE_WALKING 100
#define ENEMY_STATE_DEAD 200
#define BAT_STATE_DEAD 300

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
#define SUPER_BAT_BBOX_WIDTH 150
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
	DWORD timeToRespawn;
	bool isBurning = false;
	bool isDead = false;
	bool isWaitingToRespawn = false;
	int HP;
	int score;
	int damage;
	float gravity = 0.0f;
	EnemyType enemyType;

	CEnemy();
	void startBurning() { isBurning = true; burningStart = GetTickCount(); }
	void waitingToRepawn() { isWaitingToRespawn = true; respawnTime = GetTickCount(); }
	virtual void respawn() = 0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
};