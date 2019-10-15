#include "Whip.h"

Whip::Whip() : CGameObject()
{
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
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

void Whip::Render()
{
	SetPosition();
	Render(1);
}

void Whip::Render(int currentID)
{
	int ani = 1;
	animations[currentID]->Render(x, y, 255);
	//RenderBoundingBox();
}


void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != LONG_CHAIN)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != LONG_CHAIN)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}

	if (state != LONG_CHAIN)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;
}

void Whip::setPosition()
{
	CSimon *simon = CSimon::GetInstance();
	float x, y;
	simon->GetPosition(x, y);
	this->x = x;
	this->y = y;
}

void Whip::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_WHIP, L"textures\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texWhip = textures->Get(ID_TEX_WHIP);

	// big
	sprites->Add(40001, 0, 0, 0 + WHIP_WIDTH, 0 + WHIP_HEIGHT, texWhip);		// right
	sprites->Add(40002, 240, 0, 240 + WHIP_WIDTH, 0 + WHIP_HEIGHT, texWhip);
	sprites->Add(40003, 480, 0, 480 + WHIP_WIDTH, 0 + WHIP_HEIGHT, texWhip);

	sprites->Add(40004,  0,  66,   0 + WHIP_WIDTH, 66 + WHIP_HEIGHT, texWhip);		// left
	sprites->Add(40005, 240, 66, 240 + WHIP_WIDTH, 66 + WHIP_HEIGHT, texWhip);
	sprites->Add(40006, 480, 66, 480 + WHIP_WIDTH, 66 + WHIP_HEIGHT, texWhip);

	LPANIMATION ani;

	ani = new CAnimation(100);	// right 
	ani->Add(40001);
	ani->Add(40002);
	ani->Add(40003);
	animations->Add(700, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40004);
	ani->Add(40005);
	ani->Add(40006);
	animations->Add(701, ani);

	AddAnimation(700);		// idle right
	AddAnimation(701);		// idle left
}



