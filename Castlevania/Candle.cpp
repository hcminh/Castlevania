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
	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(250);		// big
	AddAnimation(251);		// small
	AddAnimation(252);		// big die

}
