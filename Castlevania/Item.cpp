#include "Item.h"
#include "Simon.h"
#include "Scenes.h"

CItem::CItem(ItemType itemType, ItemState itemState)
{
	type = ObjectType::ITEM;
	this->itemState = itemState;
	item = itemType;
	isEnable = true;
	isDestroyed = false;
	isDestroying = false;
	//state of obj
	switch (itemState)
	{
	case STATE_NONE:
		break;
	case STATE_BIG_CANDLE:
		widthState = CANDLE_BIG_BBOX_WIDTH;
		heigthState = CANDLE_BIG_BBOX_HEIGHT;
		AddAnimation(250);		// big CANDLE
		break;
	case STATE_SMALL_CANDLE:
		widthState = CANDLE_SMALL_BBOX_WIDTH;
		heigthState = CANDLE_SMALL_BBOX_HEIGHT;
		AddAnimation(251);		// small CANDLE
		break;
	case STATE_BRICK:
		widthState = 32;
		heigthState = 32;
		AddAnimation(261);		// brick item
		break;
	case STATE_WALL:
		widthState = 32;
		heigthState = 32;
		AddAnimation(262);		// wall item
		break;
	default:
		break;
	}
	//item that obj holding
	switch (item)
	{
	case TYPE_NONE:
		AddAnimation(267);		// big heart
		//vy = BIG_HEART_GRAVITY;
		width = 0;
		heigth = 0;
		break;
	case BIG_HEART:
		AddAnimation(253);		// big heart
		vy = BIG_HEART_GRAVITY;
		width = BIG_HEART_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case SMALL_HEART:
		AddAnimation(254);		// small heart
		velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
		vy = SMALL_HEART_GRAVITY;
		width = SMALL_HEART_BBOX;
		heigth = SMALL_HEART_BBOX;
		break;
	case WHIP:
		AddAnimation(255);		// whip
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = WEAPON_BBOX;
		break;
	case KNIFE:
		AddAnimation(256);		// knife
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case AXE:
		AddAnimation(257);		// AXE
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case STOP_WATCH:
		AddAnimation(258);		// STOP_WATCH
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case INVISIBLE:
		AddAnimation(259);		// INVISIBLE
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case HOLY_WATER:
		AddAnimation(260);		// HOLY_WATER
		vy = BIG_HEART_GRAVITY;
		width = WEAPON_BBOX;
		heigth = BIG_HEART_BBOX;
		break;
	case POINT_400:
		AddAnimation(263);		// túi xanh
		vy = BIG_HEART_GRAVITY;
		width = PACKAGE_BBOX;
		heigth = PACKAGE_BBOX;
		break;
	case POINT_700:
		AddAnimation(264);		// túi vàng
		vy = BIG_HEART_GRAVITY;
		width = PACKAGE_BBOX;
		heigth = PACKAGE_BBOX;
		break;
	case POINT_1000:
		AddAnimation(265);		// túi đỏ
		vy = BIG_HEART_GRAVITY;
		width = PACKAGE_BBOX;
		heigth = PACKAGE_BBOX;
		break;
	case FOOD:
		AddAnimation(266);		// GÀ NƯỚNG
		vy = BIG_HEART_GRAVITY;
		width = PACKAGE_BBOX;
		heigth = PACKAGE_BBOX;
		break;
	case DOUBLE_SHOT:
		AddAnimation(268);		// double shot
		vy = BIG_HEART_GRAVITY;
		width = PACKAGE_BBOX;
		heigth = PACKAGE_BBOX;
		break;
	default:
		break;
	}

	AddAnimation(252);		// DESTROY
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

	if (item == ItemType::SMALL_HEART && vy != 0)
	{
		vx += velocityVariation_x;
		vy += SMALL_HEART_GRAVITY * dt;
		if (vx >= ITEM_FALLING_SPEED_X || vx <= -ITEM_FALLING_SPEED_X)
			velocityVariation_x *= -1; // đổi chiều
	}

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
		x += dx;
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
	if (isDestroying) ani = ITEM_ANI_DETROY;
	else if (!isDestroyed) ani = ITEM_ANI_STATE;
	else ani = ITEM_ANI_ITEM;
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isDestroyed)
	{
		l = x;
		t = y;
		r = x + widthState;
		b = y + heigthState;
	}
	else
	{
		l = x;
		t = y;
		r = x + width;
		b = y + heigth;
	}
}

void CItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANDLE_STATE_DESTROYING:
		if (isDestroying || isDestroyed) return;
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

