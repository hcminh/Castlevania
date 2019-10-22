#include "Item.h"
#include "Simon.h"

CItem::CItem(ItemType itemType)
{
	type = ObjectType::ITEM;
	item = itemType;
	timeDisplayed = 0;
	timeWaited = 0;
	isFinish = 0;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 120 - BIG_HEART_BBOX;
	LoadResources();
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable) return;
	if (timeWaited < TIME_DELAY_ITEM)
	{
		timeWaited += dt;
		return;
	}

	timeDisplayed += dt;
	if (timeDisplayed >= TIME_APPEAR_ITEM)
	{
		isEnable = false;
		return;
	}

	// Simple fall down
	vy += BIG_HEART_GRAVITY * dt;
	CGameObject::Update(dt);

	if (CSimon::GetInstance()->isColisionItem(this))
	{
		isEnable = false;
		CSimon::GetInstance()->colisionItem(this);
	}

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
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CItem::Render()
{
	if (timeWaited < TIME_DELAY_ITEM || timeDisplayed >= TIME_APPEAR_ITEM || !isEnable) return;
	int ani;
	if (item == ItemType::BIG_HEART)
		ani = ITEM_BIG_HEART;
	else if(item == ItemType::SMALL_HEART)
		ani = ITEM_SMALL_HEART;
	else if (item == ItemType::WHIP)
		ani = ITEM_WHIP;
	else if (item == ItemType::KNIFE)
		ani = ITEM_KNIFE;
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIG_HEART_BBOX;
	b = y + BIG_HEART_BBOX;
}

void CItem::LoadResources()
{
	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(300);		// big heart
	AddAnimation(301);		// small heart
	AddAnimation(302);		// whip
	AddAnimation(303);		// knife

}
