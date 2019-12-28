#include "Fish.h"
#include "Camera.h"
#include "GameObject.h"
#include "Simon.h"
#include "Water.h"
#include "FireBall.h"
#include "debug.h"


CFish::CFish() : CEnemy()
{
	isEnable = true;

	type = ObjectType::ENEMY;
	width = FISH_BBOX_WIDTH;
	height = FISH_BBOX_HEIGHT;
	gravity = FISH_GRAVITY;

	weapon = new CFireBall(1);

	AddAnimation(304);	//cá bắn phải
	AddAnimation(305);	//cá bắn trái
	AddAnimation(306);	//cá đứng phải
	AddAnimation(307);	//cá đứng trái
	AddAnimation(308);	//cá đi phải
	AddAnimation(309);	//cá đi trái
	AddAnimation(252);		// DEAD
	AddAnimation(252);		// cho 2 cái dead để áp dụng công thức ani += (nx > 0) ? 0 : 1;

	dead();
	waitingToRepawn();
}

void CFish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (isDead)
	{
		if (isWaitingToRespawn && (GetTickCount() - respawnTime > ENEMY_RESPAWN_TIME))	//enemy fire
		{
			respawnTime = 0;
			isWaitingToRespawn = false;
			respawn();
		}
		else return;
	}
	CGameObject::Update(dt, coObject);
	vy += gravity * dt;

	if (canShoot())
	{
		waitToShoot = 0;
		isWaitToShoot = false;
		SetState(FISH_STATE_SHOOT);

	}
	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		dead();
		waitingToRepawn();
	}
	else if (isShooting && (GetTickCount() - shootingTime > FISH_SHOOTING_TIME))	//enemy fire
	{
		shootingTime = 0;
		isShooting = false;
		nx *= -1;
		SetState(FISH_STATE_WALK);
		waitingToShoot();
	}
	else
	{
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
			float min_tx, min_ty, nx = 0, ny;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			x += min_tx * dx + nx * 0.1f; // để vầy là khỏi cần kiểm tra va chạm nx
			y += min_ty * dy + ny * 0.1f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (e->obj->type == ObjectType::GROUND)
				{

					if (e->nx != 0)
					{
						this->nx *= -1;
						SetState(FISH_STATE_WALK);
					}
					if (e->ny != 0)
					{
						if (e->ny < 0.0f && isFlying)
						{
							vy = 0;
							active();
						}
						else if (e->ny > 0.0f && isFlying)
							y += dy;
					}
				}
				else if (e->obj->type == ObjectType::WATER)
				{
					if (e->ny == -1.0f)
					{
						auto * water = dynamic_cast<CWater*>(e->obj);
						water->AddBubbles(x, y + width);
						burningStart = 0;
						isBurning = false;
						dead();
						waitingToRepawn();
					}
					else if (e->ny == 1.0f)
					{
						auto * water = dynamic_cast<CWater*>(e->obj);
						water->AddBubbles(x, y - width);
						y += dy;
					}
				}
			}

		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		/*if (!CCamera::GetInstance()->onCamera(x, x + width))
		{
			dead();
			waitingToRepawn();
		}*/
	}

	if (weapon != NULL) weapon->Update(dt, coObject);

}

void CFish::Render()
{
	if (isDead) return;
	int ani = FISH_ANI_IDLE_RIGHT;
	if (isBurning) ani = FISH_ANI_DEAD;
	else if (isWalking) ani = FISH_ANI_WALK_RIGHT;
	else if (isShooting) ani = FISH_ANI_SHOOT_RIGHT;
	else if (isFlying) ani = FISH_ANI_IDLE_RIGHT;

	ani += (nx > 0) ? 0 : 1;

	if (weapon->isFlying) weapon->Render();
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CFish::respawn()
{
	vx = 0;
	vy = -1.15f;
	nx = -1;
	gravity = FISH_GRAVITY;
	isDead = false;
	isWalking = false;
	isFlying = true;
	float respawPosX = rand() % (int)CCamera::GetInstance()->getBorderCamRight() + CCamera::GetInstance()->getBorderCamLeft();
	SetPosition(respawPosX, 500);
}

void CFish::dead()
{
	vy = vx = 0;
	gravity = 0;
	isDead = true;
	isShooting = false;
	isFlying = false;
	isWalking = false;
	isWaitToShoot = false;
	burningStart = 0;
	isBurning = false;
	weapon->burningStart = 0;
	weapon->isFlying = false;
}

void CFish::active()
{
	vy = 0;
	isFlying = false;
	SetState(FISH_STATE_WALK);
	nextShootTime = rand() % 2500 + 500;
	waitingToShoot();
}

bool CFish::canShoot()
{
	return (state != ENEMY_STATE_DEAD && isWaitToShoot &&
		GetTickCount() - waitToShoot >= nextShootTime && weapon->isFlying == false);
}

void CFish::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FISH_STATE_WALK:
		isWalking = true;
		vx = nx * FISH_WALKING_SPEED;
		break;
	case FISH_STATE_SHOOT:
		vx = 0;
		nextShootTime = rand() % 2500 + 500;
		isWalking = false;
		startShoot();
		if (!weapon->isFlying)
		{
			weapon->nx = nx;
			weapon->SetPosition(x, y);
			weapon->SetState(FIRE_BALL_STATE_SHOOT);
		}
		break;
	case ENEMY_STATE_DEAD:
		waitToShoot = 0;
		isWaitToShoot = false;
		vx = 0;
		vy = 0;
		gravity = 0;
		startBurning();
		break;
	}
}




