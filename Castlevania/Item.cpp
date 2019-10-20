#include "Item.h"

CItem::CItem(ItemType type)
{
	type = ItemType::BIG_HEART;
	TimeDisplayed = 0;
	TimeWaitMax = 0;
	TimeDisplayMax = 0;
	TimeWaited = 0;
	isFinish = 0;
}

void CItem::Render()
{
	animations[0]->Render(x, y);
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + ITEM_BBOX_WIDTH;
	b = y + ITEM_BBOX_HEIGHT;
}


bool CItem::isWaitingDisplay()
{
	return TimeWaited < TimeWaitMax;
}


void CItem::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_ITEM, L"textures\\Items.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texItem = textures->Get(ID_TEX_ITEM);

	// heart
	sprites->Add(60000, 16,	0,	40,	20, texItem);

	LPANIMATION ani;

	ani = new CAnimation(100);	// heart
	ani->Add(60000);
	animations->Add(900, ani);

	AddAnimation(800);		// heart

}
