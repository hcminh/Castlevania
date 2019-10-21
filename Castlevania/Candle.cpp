#include "Candle.h"
#include "Scenes.h"

CCandle::CCandle(float x, float y, ItemType item)
{
	type = ObjectType::CANDLE;
	this->item = item;
	LoadResources();
	SetPosition(x, y);
}

CCandle::~CCandle()
{
}

void CCandle::Render()
{
	if (!isEnable) return;
	int ani;
	if (state == CANDLE_DESTROYED)
	{
		ani = CANDLE_BIG_ANI_DESTROYED;
	}
	else
	{
		ani = CANDLE_BIG;
	}
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable) return;

	if (state == CANDLE_DESTROYED)
	{
		timeWaited += dt;
		if (timeWaited >= TIME_DESTROY_MAX)
			isEnable = false;
		return;
	}
}

void CCandle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isEnable) return;
	l = x;
	t = y;
	r = x + CANDLE_BIG_BBOX_WIDTH;
	b = y + CANDLE_BIG_BBOX_HEIGHT;
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANDLE_DESTROYED:
		//isEnable = false;
		if (destroyed) return;
		destroyed = true;
		CScenes::GetInstance()->putItem(this->item, this->x, this->y);
		break;
	default:
		break;
	}
}

void CCandle::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_CANDLE, L"textures\\candle-full.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texCandle = textures->Get(ID_TEX_CANDLE);

	// big
	sprites->Add(50001, 0,  0,  0 + CANDLE_BIG_WIDTH, 0 + CANDLE_BIG_HEIGHT, texCandle);
	sprites->Add(50002, 32, 0, 32 + CANDLE_BIG_WIDTH, 0 + CANDLE_BIG_HEIGHT, texCandle);

	//small
	sprites->Add(50003, 64, 0, 80, 32, texCandle);
	sprites->Add(50004, 80, 0, 96, 32, texCandle);

	//big die
	sprites->Add(50005, 4 * 32,	0, 5 * 32, 60, texCandle);

	LPANIMATION ani;

	ani = new CAnimation(100);	// big
	ani->Add(50001);
	ani->Add(50002);
	animations->Add(800, ani);

	ani = new CAnimation(100);	// small
	ani->Add(50003);
	ani->Add(50004);
	animations->Add(801, ani);

	ani = new CAnimation(100);	// big die
	ani->Add(50005);
	animations->Add(802, ani);

	AddAnimation(800);		// big
	AddAnimation(801);		// small
	AddAnimation(802);		// big die

}
