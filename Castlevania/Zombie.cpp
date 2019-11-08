#include "Zombie.h"

void CZombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;

	if (state == ZOMBIE_STATE_DIE)
		bottom = y + ZOMBIE_BBOX_HEIGHT_DIE;
	else
		bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CZombie::Render()
{
	int ani = ZOMBIE_ANI_WALKING;
	if (state == ZOMBIE_STATE_DIE) {
		ani = ZOMBIE_ANI_DIE;
	}

	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CZombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_DIE:
		y += ZOMBIE_BBOX_HEIGHT - ZOMBIE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case ZOMBIE_STATE_WALKING:
		vx = -ZOMBIE_WALKING_SPEED;
	}

}

void CZombie::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	LPANIMATION ani;


	ani = new CAnimation(100);		// gooma
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(701, ani);

	ani = new CAnimation(100);		// gooma die
	ani->Add(30003);
	animations->Add(702, ani);


	AddAnimation(701);
	AddAnimation(702);
}
