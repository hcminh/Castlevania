#include "Zombie.h"
#include "Camera.h"


CZombie::CZombie() : CEnemy()
{
	isEnable = true;
	isDead = false;
	width = ZOMBIE_BBOX_WIDTH;
	height = ZOMBIE_BBOX_HEIGHT;
	vx = ZOMBIE_WALKING_SPEED;
	AddAnimation(310);	//ma đi phải
	AddAnimation(311);	//ma đi trái
	AddAnimation(252);		// DEAD
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt, coObject);

	//if (isRespawning && (GetTickCount() - respawnTime > ENEMY_RESPAWN_TIME)) // thoi gian hoi sinh
	//{
	//	respawnTime = 0;
	//	isRespawning = false;
	//	respawn();
	//}
	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		SetState(ZOMBIE_STATE_RESPAWN);
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
	vx = ENEMY_WALKING_SPEED;
	//SetPosition(CCamera::GetInstance()->getBorderCamLeft(), 300);
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




