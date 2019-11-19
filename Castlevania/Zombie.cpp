#include "Zombie.h"


CZombie::CZombie() : CEnemy()
{
	isEnable = true;
	width = ZOMBIE_BBOX_WIDTH;
	height = ZOMBIE_BBOX_HEIGHT;
	vx = ZOMBIE_WALKING_SPEED;
	AddAnimation(310);	//ma đi phải
	AddAnimation(311);	//ma đi trái
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt, coObject);

	x += dx;
	y += dy;

	if (x <= 20 || x >= 300)
	{
		nx = -nx;
		vx = vx * nx;
	}
}

void CZombie::Render()
{
	int ani;
	if (nx > 0)
		ani = 0;
	else ani = 1;
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderBoundingBox();
}

void CZombie::SetState(int state)
{
	switch (state)
	{
	case ENEMY_STATE_WALKING:
		vx = -ENEMY_WALKING_SPEED;
		break;
	}
}




