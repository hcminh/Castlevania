#include "Enemy.h"


CEnemy::CEnemy(EnemyType eType) {
	enemyType = eType;
	isEnable = true;
	switch (enemyType)
	{
	case GHOST:
		width = GHOST_BBOX_WIDTH;
		height = GHOST_BBOX_HEIGHT;
		vx = ENEMY_WALKING_SPEED;
		AddAnimation(310);	//ma đi phải
		AddAnimation(311);	//ma đi trái
		break;
	case DOG:
		width = DOG_BBOX_WIDTH;
		height = DOG_BBOX_HEIGHT;
		vx = -ENEMY_WALKING_SPEED;
		AddAnimation(300);	//chó NGỒI R
		AddAnimation(301);	//chó NGỒI L
		AddAnimation(302);	//chó đi phải
		AddAnimation(303);	//chó đi trái
		break;
	case BAT:
		width = BAT_BBOX;
		height = BAT_BBOX;
		vx = -ENEMY_WALKING_SPEED;
		AddAnimation(312);	//dơi đậu
		AddAnimation(313);	//dơi bay phải
		AddAnimation(314);	//dơi bay trái
		break;
	case FISH:
		width = GHOST_BBOX_WIDTH;
		height = GHOST_BBOX_HEIGHT;
		vx = -ENEMY_WALKING_SPEED;
		AddAnimation(304);	//cá bắn phải
		AddAnimation(305);	//cá bắn trái
		AddAnimation(306);	//cá đứng phải
		AddAnimation(307);	//cá đứng trái
		AddAnimation(308);	//cá đi phải
		AddAnimation(309);	//cá đi trái
		break;
	case SUPPER_BAT:
		width = SUPER_BAT_BBOX_WIDTH;
		height = SUPER_BAT_BBOX_HEIGHT;
		vx = -ENEMY_WALKING_SPEED;
		AddAnimation(315);	//dơi trùm đậu
		AddAnimation(316);	//dơi trùm vỗ cánh
		break;
	default:
		break;
	}
}

void CEnemy::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (x <= 20 || x >= 300)
	{
		nx = -nx;
		vx = vx * nx;
	}
}

void CEnemy::Render()
{
	int ani;
	if(nx > 0)
		ani = 0;
	else ani = 1;
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderBoundingBox();
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_WALKING:
		vx = -ENEMY_WALKING_SPEED;
		break;
	}

}

