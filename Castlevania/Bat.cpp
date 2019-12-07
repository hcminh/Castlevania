#include "Bat.h"
#include "Camera.h"
#include "Debug.h"

CBat::CBat(DWORD timeToRespawn, int nx) : CEnemy()
{
	isEnable = true;
	this->nx = nx;
	this->timeToRespawn = timeToRespawn;
	velVariation = BAT_SPEED_VARIATION;
	width = BAT_BBOX;
	height = BAT_BBOX;
	vx = -ENEMY_WALKING_SPEED;
	vy = 0.08f;
	AddAnimation(313);	//dơi bay phải
	AddAnimation(314);	//dơi bay trái
	AddAnimation(252);		// DEAD

	dead();
	waitingToRepawn();
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (isDead)
	{
		if (isWaitingToRespawn && (GetTickCount() - respawnTime > timeToRespawn))	//enemy respawn
		{
			respawnTime = 0;
			isWaitingToRespawn = false;
			respawn();
		}
		else return;
	}

	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		dead();
		waitingToRepawn();
	}
	else
	{
		CGameObject::Update(dt);

		vy += velVariation;

		if (vy >= BAT_FLYING_SPEED_Y || vy <= -BAT_FLYING_SPEED_Y)
			velVariation *= -1;

		x += dx;
		y += dy;
	}

}

void CBat::Render()
{
	if (isDead) return;
	int ani;
	if (isBurning) ani = BAT_ANI_DEAD;
	else if (nx > 0)
		ani = 0;
	else ani = 1;

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CBat::respawn()
{
	isDead = false;
	vx = BAT_FLYING_SPEED_X *nx;
	vy = 0.08f;

	if (nx > 0)
		SetPosition(CCamera::GetInstance()->getBorderCamLeft() + 50, 50 + rand() % 100);
	else
		SetPosition(CCamera::GetInstance()->getBorderCamRight() - 50, 50 + rand() % 100);
}

void CBat::dead()
{
	isDead = true;
	vx = 0;
	vy = 0;
	SetPosition(0, 450);
}

void CBat::SetState(int state)
{
	switch (state)
	{
	case ENEMY_STATE_WALKING:
		if (nx > 0) vx = BAT_FLYING_SPEED_X;
		else vx = -BAT_FLYING_SPEED_X;
		vy = 0;
		break;
	case BAT_STATE_DEAD:
		vx = 0;
		vy = 0;
		gravity = 0;
		startBurning();
		break;
	case BAT_STATE_RESPAWN:
		//startRespawnTime();
		break;
	}
}




