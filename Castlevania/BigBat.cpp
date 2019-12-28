#include "BigBat.h"
#include "Simon.h"
#include "Camera.h"

CBigBat::CBigBat(float x, float y) : CEnemy()
{
	this->isEnable = true;
	this->enemyType = EnemyType::SUPPER_BAT;
	this->type = ObjectType::ENEMY;
	this->SetPosition(x, y);
	SetState(BIG_BAT_STATE_IDLE);

	AddAnimation(315);	//dơi trùm đậu
	AddAnimation(316);	//dơi trùm vỗ cánh
	AddAnimation(252);		// CHÁY


	width = BIG_BAT_BBOX_WIDTH;
	height = BIG_BAT_BBOX_HEIGHT;

	isFlyToTarget = false;
	isFlyToSimon = false;

	startTimeWaiting = 0;
	isStopWaiting = false;

}

void CBigBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	if (state == BIG_BAT_STATE_IDLE || isDead)
	{
		if (abs(CSimon::GetInstance()->x - this->x) < 100)
		{
			SetState(BIG_BAT_STATE_ACTIVE);
		}
		else return;
	}

	if (GetTickCount() - attackTime > BIG_BAT_ATTACK_TIME_WAITING)	//enemy fire
	{
		attackTime = 0;
		isFlying = true;
		startAttack();
		flyToSimonPercent = rand() % 3 + 1; // ==1 thi flytosimon
		if (flyToSimonPercent == 1)
		{
			CSimon::GetInstance()->GetPosition(newPosition.x, newPosition.y);
		}
		else
		{
			randomNewPosition();
		}

	}

	if (isFlying)
	{
		if (this->x > newPosition.x) vx = -0.15f;
		else vx = 0.15f;
		if (this->y > newPosition.y) vy = -0.15f;
		else vy = 0.15f;
		FlyToTarget(dt, newPosition);
	}
}

void CBigBat::Render()
{
	if (isDead) return;
	int ani = BIG_BAT_ANI_IDLE;
	if (state == BIG_BAT_STATE_IDLE) ani = BIG_BAT_ANI_IDLE;
	else if (isBurning) ani = BIG_BAT_ANI_BURNING;
	else ani = BIG_BAT_ANI_FLYING;

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CBigBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BIG_BAT_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BIG_BAT_STATE_ACTIVE:
		xCam = CCamera::GetInstance()->camX;
		vx = 0.1f;
		vy = 0.05f;
		startAttack();
		break;
	case BIG_BAT_STATE_FLY_AWAY:
		randomNewPosition();
		isFlying = true;
		attackTime = 0;
		break;
	default:
		break;
	}
}

void CBigBat::FlyToTarget(DWORD dt, D3DXVECTOR2 target)
{
	x += vx * dt;
	y += vy * dt;

	if (abs(x - target.x) <= 1.0f && abs(y - target.y) <= 1.0f)
	{
		isFlyToTarget = false;
		this->SetPosition(target.x, target.y);
		isFlying = false;
	}
}

void CBigBat::Flying(DWORD dt)
{
	flyToSimonPercent = rand() % 3 + 1; // ==1 thi flytosimon

	if (flyToSimonPercent == 1)
	{
		CSimon::GetInstance()->GetPosition(simonPostion.x, simonPostion.y);
		FlyToTarget(dt, simonPostion);
	}
	else
	{
		randomNewPosition();
		FlyToTarget(dt, newPosition);
	}
}

void CBigBat::randomNewPosition()
{
	srand(time(NULL));
	newPosition.x = rand() % (SCREEN_WIDTH - BIG_BAT_BBOX_WIDTH) + xCam;
	newPosition.y = rand() % (SCREEN_HEIGHT - 160) + 80;
}

void CBigBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x; 
	top = y;
	right = left + BIG_BAT_BBOX_WIDTH;
	bottom = top + BIG_BAT_BBOX_HEIGHT;
}



