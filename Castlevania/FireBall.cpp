#include "FireBall.h"
#include "Camera.h"
#include "Simon.h"
#include "Scenes.h"
#include "Debug.h"


CFireBall::CFireBall(int nx) : CEnemy()
{
	isEnable = true;
	type = ENEMY;
	this->nx = nx;
	vx = 0.2f;
	width = FIRE_BALL_BBOX_WIDTH;
	height = FIRE_BALL_BBOX_HEIGHT;

	AddAnimation(207);
	AddAnimation(208);
	AddAnimation(252);		// DEAD
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (!isFlying) return;

	if (isBurning && (GetTickCount() - burningStart > ENEMY_BURN_TIME))	//enemy fire
	{
		burningStart = 0;
		isBurning = false;
		isFlying = false;
	}
	CGameObject::Update(dt, coObject);
	x += dx;
	if (x > CCamera::GetInstance()->getCamPosX() + SCREEN_WIDTH || x < CCamera::GetInstance()->getCamPosX())
	{
		burningStart = 0;
		isBurning = false;
		isFlying = false;
	}
	else
	{
		CScenes::GetInstance()->pushObjToCam(this);
	}

}

void CFireBall::Render()
{
	if (isDead) return;
	int ani;
	if (isBurning) ani = FIRE_BALL_ANI_DEAD;
	else if (nx > 0)
		ani = 0;
	else ani = 1;

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CFireBall::respawn()
{
}

void CFireBall::SetState(int state)
{
	switch (state)
	{
	case ENEMY_STATE_DEAD:
		vx = 0;
		vy = 0;
		gravity = 0;
		startBurning();
		break;
	case FIRE_BALL_STATE_DEAD:
		burningStart = 0;
		isBurning = false;
		isFlying = false;
		break;
	case FIRE_BALL_STATE_SHOOT:
		if (nx == -1) vx = -0.2f;
		else if (nx == 1) vx = 0.2f;
		isFlying = true;
		break;
	}
}

void CFireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (isBurning)
	{
		left = x;
		top = y;
		right = left;
		bottom = top;
	}
	else 
	{
		left = x + 10.0f;
		top = y + 10.0f;
		right = left + 15.0f; //sài lại cho lẹ
		bottom = top + 15.0f;
	}

}

