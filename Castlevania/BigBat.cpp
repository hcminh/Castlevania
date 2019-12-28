#include "BigBat.h"

CBigBat::CBigBat(float x, float y) : CEnemy()
{
	this->isEnable = true;
	this->enemyType = EnemyType::SUPPER_BAT;
	this->type = ObjectType::BIG_BAT;
	this->SetPosition(x, y);
	SetState(BIG_BAT_STATE_IDLE);

	AddAnimation(315);	//dơi trùm đậu
	AddAnimation(316);	//dơi trùm vỗ cánh
	AddAnimation(252);		// CHÁY


	width = BIG_BAT_BBOX_WIDTH;
	height = BIG_BAT_BBOX_HEIGHT;

	isFlyToTarget = false;
	isFlyToSimon = false;

	idTarget = 0;
	startTimeWaiting = 0;
	isStopWaiting = false;

}

void CBigBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	if (state == BIG_BAT_STATE_IDLE || isDead)
		return;

	if (isFlyToTarget == false)
	{
		isFlyToTarget = true;

		// deternmind target
		if (idTarget == 1) // tỉ lệ bay vao simon là 33%
		{
			isFlyToSimon = true;
			target = simonPostion;
		}
		else
		{
			target = GetRandomSpot();
		}

		GetVelocity();
	}
	else
	{
		FlyToTarget(dt);
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
	RenderBoundingBox();
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

	default:
		break;
	}
}

D3DXVECTOR2 CBigBat::GetRandomSpot()
{
	D3DXVECTOR2 randomSpot;

	float left = x - BIG_BAT_RECT_RANDOMSPOT_BBOX_WIDTH;
	float top = y;

	float distance = 0;

	do // chọn điểm random sao cho quãng đường bay dài dài một tí
	{
		randomSpot.x = left + rand() % (400);
		randomSpot.y = top + rand() % (BIG_BAT_RECT_RANDOMSPOT_BBOX_HEIGHT);

		float dx = abs(x - randomSpot.x);
		float dy = abs(y - randomSpot.y);

		distance = sqrt(pow(x - randomSpot.x, 2) + pow(y - randomSpot.y, 2));
	} while (distance < 100.0f);

	return randomSpot;
}

void CBigBat::FlyToTarget(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	if (abs(x - target.x) <= 1.0f)
	{
		isFlyToTarget = false;
		this->SetPosition(target.x, target.y);

		idTarget = (idTarget + 1) % 3;

		if (isFlyToSimon == true)
		{
			isFlyToSimon = false;
		}
		else
		{
			StartStopTimeCounter();
		}
	}
}

void CBigBat::GetVelocity()
{
	float dx = abs(x - target.x);
	float dy = abs(y - target.y);

	// lấy phương hướng
	int nx, ny;

	if (x < target.x) nx = 1;
	else nx = -1;

	if (y < target.y) ny = 1;
	else ny = -1;

	// tính vận tốc
	if (isFlyToSimon == true)
	{
		vx = nx * dx / 1500;
		vy = ny * dy / 1500;// BIG_BAT_FAST_TIME_TO_FLY;
	}
	else
	{
		vx = nx * dx / 2000;
		vy = ny * dy / 2000;
	}
}

void CBigBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x; 
	top = y;
	right = left + BIG_BAT_BBOX_WIDTH;
	bottom = top + BIG_BAT_BBOX_HEIGHT;
}



