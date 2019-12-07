#pragma once

#include "Enemy.h"

// Zombie

#define BAT_FLYING_SPEED_X	0.12f
#define BAT_FLYING_SPEED_Y	0.1f
#define BAT_SPEED_VARIATION 0.004f

#define BAT_BBOX_WIDTH			30
#define BAT_BBOX_HEIGHT			30

#define BAT_STATE_DEAD			0
#define BAT_STATE_RESPAWN		1

#define BAT_ANI_WALK_RIGHT		0	//ma đi phải
#define BAT_ANI_WALK_LEFT		1	//ma đi trái
#define BAT_ANI_DEAD				2	//ma đi trái

class CBat : public CEnemy
{
public:
	float velVariation;
	CBat(DWORD timeToRespawn, int nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void respawn();
	virtual void dead();
	virtual void SetState(int state);
};

