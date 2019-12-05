#include "Zombie.h"
#include "Camera.h"
#include "Debug.h"


CZombie::CZombie(DWORD timeToRespawn, int nx) : CEnemy()
{
	isEnable = true;
	this->nx = nx;
	this->timeToRespawn = timeToRespawn;
	gravity = ZOMBIE_GRAVITY;
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
		CGameObject::Update(dt, coObject);
		vy += gravity * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(coObject, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			if (nx != 0)
			{
				this->nx *= -1; // chạm tường thì đổi chiều
				this->vx *= -1;
			}
		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

		if (!CCamera::GetInstance()->onCamera(x, x + width))
		{
			dead();
			waitingToRepawn();
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
	gravity = ZOMBIE_GRAVITY;
	vx = nx * ZOMBIE_WALKING_SPEED;
	if (nx > 0)
		SetPosition(CCamera::GetInstance()->getBorderCamLeft() + 50, 304);
	else
		SetPosition(CCamera::GetInstance()->getBorderCamRight() - 50, 304);
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
		vx = nx * ZOMBIE_WALKING_SPEED;
		break;
	case ZOMBIE_STATE_DEAD:
		vx = 0;
		vy = 0;
		gravity = 0;
		startBurning();
		break;
	case ZOMBIE_STATE_RESPAWN:
		//startRespawnTime();
		break;
	}
}




