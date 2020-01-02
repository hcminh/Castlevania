#include "Enemy.h"


CEnemy::CEnemy() {
	type = ObjectType::ENEMY;
}

void CEnemy::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isDead || isBurning)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	else
	{
		//left = x;
		//top = y;
		//right = left + width;
		//bottom = top + height;
		left = x + 11;  // (10/32)
		top = y + 2; // (60,64)
		right = left + 10;
		bottom = top + 60;
	}
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}

void CEnemy::Render()
{
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
}

