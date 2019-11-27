#include "Zombie.h"
#include "Camera.h"


CZombie::CZombie() : CEnemy()
{
	isEnable = true;
	width = ZOMBIE_BBOX_WIDTH;
	height = ZOMBIE_BBOX_HEIGHT;
	AddAnimation(310);	//ma đi phải
	AddAnimation(311);	//ma đi trái
	AddAnimation(252);		// DEAD

	dead();
	waitingToRepawn();
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (isDead)
	{
		if (isWaitingToRespawn && (GetTickCount() - respawnTime > ENEMY_RESPAWN_TIME))	//enemy respawn
		{
			respawnTime = 0;
			isWaitingToRespawn = false;
			respawn();
		}
		else return;
	}
	CGameObject::Update(dt, coObject);

	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		dead();
		waitingToRepawn();
	}
	else
	{
		x += dx;
		y += dy;

		if (x <= 20 || x >= 300)
		{
			nx = -nx;
			vx = vx * nx;
		}
	}

}

void CZombie::Render()
{
	if (isDead) return;
	int ani;
	if (isBurning) ani = ZOMBIE_ANI_DEAD;
	else if (nx > 0)
		ani = 0;
	else ani = 1;

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CZombie::respawn()
{
	isDead = false;
	vx = ENEMY_WALKING_SPEED;
	SetPosition(200, 304);
	//SetPosition(CCamera::GetInstance()->getBorderCamLeft(), 300);
}

void CZombie::dead()
{
	isDead = true;
	vx = 0;
	SetPosition(0, 450);
}

void CZombie::active()
{
	SetState(ENEMY_STATE_WALKING);
}

void CZombie::SetState(int state)
{
	switch (state)
	{
	case ENEMY_STATE_WALKING:
		vx = -ENEMY_WALKING_SPEED;
		break;
	case ZOMBIE_STATE_DEAD:
		vx = 0;
		startBurning();
		break;
	case ZOMBIE_STATE_RESPAWN:
		//startRespawnTime();
		break;
	}
}




