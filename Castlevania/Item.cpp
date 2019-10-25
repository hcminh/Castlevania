#include "Item.h"
#include "Simon.h"
#include "Scenes.h"

CItem::CItem(ItemType itemType, ItemState itemState)
{
	type = ObjectType::ITEM;
	this->itemState = itemState;
	item = itemType;
	isDestroyed = false;
	isDestroying = false;
	timeDisplayed = 0;
	timeWaited = 0;
	LoadResources();
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable) return;
	CGameObject::Update(dt);
	if (isDestroying)
	{
		timeWaited += dt;
		if (timeWaited <= TIME_DELAY_ITEM) return;
		SetState(CANDLE_STATE_DESTROYED);
		return;
	}
	else if (isDestroyed)
	{
		timeDisplayed += dt;
		if (timeDisplayed >= TIME_APPEAR_ITEM) isEnable = false;
	}

	if (!isDestroyed) return;
	// Simple fall down
	vy += BIG_HEART_GRAVITY * dt;

	if (CSimon::GetInstance()->isColisionItem(this))
	{
		isEnable = false;
		CSimon::GetInstance()->colisionItem(this);
	}
	if (isTouchGround) return;
	vector<LPGAMEOBJECT> listObject_Ground;
	listObject_Ground.clear();
	for (UINT i = 0; i < coObjects->size(); i++) {
		if (coObjects->at(i)->type == ObjectType::GROUND)
			listObject_Ground.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(&listObject_Ground, coEvents); // Lấy danh sách các va chạm 
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
		{
			vy = 0;
			isTouchGround = true;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CItem::Render()
{
	if (!isEnable) return;

	int ani;
	if (isDestroying)
		ani = ITEM_CANDLE_DESTROYED;
	else if (!isDestroyed)
	{
		if (itemState == ItemState::STATE_BIG_CANDLE)
			ani = ITEM_BIG_CANDLE;
		if (itemState == ItemState::STATE_SMALL_CANDLE)
			ani = ITEM_SMALL_CANDLE;
	}
	else
	{
		if (item == ItemType::BIG_HEART)
			ani = ITEM_BIG_HEART;
		else if (item == ItemType::SMALL_HEART)
			ani = ITEM_SMALL_HEART;
		else if (item == ItemType::WHIP)
			ani = ITEM_WHIP;
		else if (item == ItemType::KNIFE)
			ani = ITEM_KNIFE;
	}
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isDestroyed)
	{
		l = x;
		t = y;
		r = x + CANDLE_BIG_BBOX_WIDTH;
		b = y + CANDLE_BIG_BBOX_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + BIG_HEART_BBOX;
		b = y + BIG_HEART_BBOX;
	}
}

void CItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANDLE_STATE_DESTROYING:
		if (isDestroying) return;
		isDestroying = true;
		timeWaited = 0;
		break;
	case CANDLE_STATE_DESTROYED:
		if (isDestroyed) return;
		isDestroyed = true;
		isDestroying = false;
		timeDisplayed = 0;
		break;
	default:
		break;
	}
}

void CItem::LoadResources()
{
	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(250);		// big CANDLE
	AddAnimation(251);		// small CANDLE
	AddAnimation(252);		// CANDLE DESTROY

	AddAnimation(300);		// big heart
	AddAnimation(301);		// small heart
	AddAnimation(302);		// whip
	AddAnimation(303);		// knife

}
