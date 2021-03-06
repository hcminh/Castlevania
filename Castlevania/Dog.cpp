﻿
#include <stdlib.h>  
#include "Dog.h"
#include "Camera.h"
#include "Simon.h"


CDog::CDog(float x, float y) : CEnemy()
{
	isEnable = true;
	type = ObjectType::ENEMY;
	SetPosition(x, y);
	isSitting = true;
	this->nx = -1;
	gravity = DOG_GRAVITY;
	width = DOG_BBOX_WIDTH;
	height = DOG_BBOX_HEIGHT;
	AddAnimation(300);	//chó NGỒI R
	AddAnimation(301);	//chó NGỒI L
	AddAnimation(302);	//chó đi phải
	AddAnimation(303);	//chó đi trái
	AddAnimation(317);	//chó nhảy phải
	AddAnimation(318);	//chó nhảy trái
	AddAnimation(252);		// CHÁY
	AddAnimation(252);		// CHÁY

}

void CDog::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		dead();
		waitingToRepawn();
	}

	if (isSitting && abs(CSimon::GetInstance()->x - this->x) < 100)
	{
		active();
	}
	vy += gravity * dt;
	CGameObject::Update(dt, coObject);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (!isJumping && (state == ENEMY_STATE_WALKING))
		{
			SetState(DOG_STATE_JUMP);
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.4f;

		if (ny == -1.0f)
		{
			vy = 0;

			if (state == DOG_STATE_JUMP)
			{
				(this->nx) *= -1;
				SetState(ENEMY_STATE_WALKING);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CDog::Render()
{
	if (isDead) return;
	int ani;
	if (isBurning) ani = DOG_ANI_BURNING;
	else if (state == DOG_STATE_JUMP) ani = DOG_ANI_JUMP_RIGHT;
	else if (isSitting) ani = DOG_ANI_SIT_RIGHT;
	else ani = DOG_ANI_WALK_RIGHT;

	ani += (nx > 0) ? 0 : 1;

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CDog::respawn()
{
	isDead = false;
	isSitting = true;
	vx = 0;
	gravity = DOG_GRAVITY;
	SetPosition(200, 340);
	//SetPosition(CCamera::GetInstance()->getBorderCamLeft(), 300);
}

void CDog::dead()
{
	isDead = true;
	vx = 0;
	SetPosition(0, 450);
}

void CDog::active()
{
	isSitting = false;
	SetState(ENEMY_STATE_WALKING);
}

void CDog::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_WALKING:
		//isJumping = false;
		vy = 0;
		vx = nx * DOG_WALKING_SPEED;
		break;
	case DOG_STATE_JUMP:
		vy = -DOG_JUMPING_SPEED;
		isJumping = true;
		break;
	case ENEMY_STATE_DEAD:
		vx = 0;
		vy = 0;
		gravity = 0;
		startBurning();
		break;
	case DOG_STATE_RESPAWN:
		//startRespawnTime();
		break;
	}
}




