#include "Candle.h"

void CCandle::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CCandle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + CANDLE_BIG_BBOX_WIDTH;
	b = y + CANDLE_BIG_BBOX_HEIGHT;
}

void CCandle::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_CANDLE, L"textures\\full-simon.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_CANDLE);

	// big
	sprites->Add(50001, 0,  0,  0 + CANDLE_BIG_WIDTH, 0 + CANDLE_BIG_HEIGHT, texSimon);
	sprites->Add(50002, 32, 0, 32 + CANDLE_BIG_WIDTH, 0 + CANDLE_BIG_HEIGHT, texSimon);

	//small
	sprites->Add(50003, 64, 0, 80, 32, texSimon);
	sprites->Add(50004, 80, 0, 96, 32, texSimon);


	LPANIMATION ani;

	ani = new CAnimation(100);	// big
	ani->Add(50001);
	ani->Add(50002);
	animations->Add(800, ani);

	ani = new CAnimation(100);	// small
	ani->Add(50003);
	ani->Add(50004);
	animations->Add(801, ani);


	AddAnimation(800);		// big
	AddAnimation(801);		// small

}
