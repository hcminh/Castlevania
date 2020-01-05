#include "Item.h"
#include "Simon.h"
#include "Scenes.h"
#include <ctime>

CItem::CItem(ItemType iType, ItemState itemState, float xPos, float yPos)
{
	type = ObjectType::ITEM;
	this->itemState = itemState;
	SetPosition(xPos, yPos);
	isEnable = true;
	isDestroyed = false;
	isDestroying = false;
	//state of obj
	switch (itemState)
	{
	case STATE_NONE:
		break;
	case STATE_BIG_CANDLE:
		width = CANDLE_BIG_BBOX_WIDTH;
		height = CANDLE_BIG_BBOX_HEIGHT;
		AddAnimation(250);		// big CANDLE
		break;
	case STATE_SMALL_CANDLE:
		width = CANDLE_SMALL_BBOX_WIDTH;
		height = CANDLE_SMALL_BBOX_HEIGHT;
		AddAnimation(251);		// small CANDLE
		break;
	case STATE_BRICK:
		width = 32;
		height = 32;
		AddAnimation(261);		// brick item
		break;
	case STATE_WALL:
		width = 32;
		height = 32;
		AddAnimation(262);		// wall item
		break;
	default:
		break;
	}
	//item that obj holding
	if (iType == RANDOM) item = randomItem();
	else item = iType;
	switch (item)
	{
	case TYPE_NONE:
		AddAnimation(267);		// none
		width = 32;
		height = 32;
		break;
	case BIG_HEART:
		AddAnimation(253);		// big heart
		gravity = ITEM_GRAVITY;
		break;
	case SMALL_HEART:
	case RANDOM:
		AddAnimation(254);		// small heart
		velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
		gravity = SMALL_HEART_GRAVITY;
		break;
	case WHIP:
		if (CSimon::GetInstance()->whip->isLevelMax())
		{
			item = SMALL_HEART;
			AddAnimation(254);		// small heart
			velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
			gravity = SMALL_HEART_GRAVITY;
			break;
		}
		else
		{
			AddAnimation(255);		// whip
			gravity = ITEM_GRAVITY;
			break;
		}
	case KNIFE:
		if (CSimon::GetInstance()->typeSubWeapon == KNIFE)
		{
			item = SMALL_HEART;
			AddAnimation(254);		// small heart
			velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
			gravity = SMALL_HEART_GRAVITY;
			break;
		}
		else
		{
			AddAnimation(256);		// knife
			gravity = ITEM_GRAVITY;
			break;
		}
	case STOP_WATCH:
		if (CSimon::GetInstance()->typeSubWeapon == STOP_WATCH)
		{
			item = SMALL_HEART;
			AddAnimation(254);		// small heart
			velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
			gravity = SMALL_HEART_GRAVITY;
			break;
		}
		else
		{
			AddAnimation(258);		// STOP_WATCH
			gravity = ITEM_GRAVITY;
			break;
		}
	case AXE:
		if (CSimon::GetInstance()->typeSubWeapon == AXE)
		{
			item = SMALL_HEART;
			AddAnimation(254);		// small heart
			velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
			gravity = SMALL_HEART_GRAVITY;
			break;
		}
		else
		{
			AddAnimation(257);		// AXE
			gravity = ITEM_GRAVITY;
			break;
		}
	case HOLY_WATER:
		if (CSimon::GetInstance()->typeSubWeapon == HOLY_WATER)
		{
			item = SMALL_HEART;
			AddAnimation(254);		// small heart
			velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
			gravity = SMALL_HEART_GRAVITY;
			break;
		}
		else
		{
			AddAnimation(260);		// HOLY_WATER
			gravity = ITEM_GRAVITY;
			break;
		}
	case HOLY_CROSS:
		AddAnimation(269);		// HOLY_CROSS
		gravity = ITEM_GRAVITY;
		break;
	case INVISIBLE:
		AddAnimation(259);		// INVISIBLE
		gravity = ITEM_GRAVITY;
		break;
	case FOOD:
		AddAnimation(266);		// GÀ NƯỚNG
		gravity = ITEM_GRAVITY;
		break;
	case POINT_400:
		AddAnimation(263);		// túi xanh
		gravity = ITEM_GRAVITY;
		break;
	case POINT_700:
		AddAnimation(264);		// túi vàng
		gravity = ITEM_GRAVITY;
		break;
	case POINT_1000:
		AddAnimation(265);		// túi đỏ
		gravity = ITEM_GRAVITY;
		break;
	case DOUBLE_SHOT:
		AddAnimation(268);		// double shot
		gravity = ITEM_GRAVITY;
		break;
	case TRIPLE_SHOT:
		AddAnimation(268);		// double shot
		gravity = ITEM_GRAVITY;
		break;
	default:
		AddAnimation(254);		// small heart
		velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
		gravity = SMALL_HEART_GRAVITY;
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

	vy += gravity * dt;
	if (item == ItemType::SMALL_HEART && vy != 0)
	{
		vx += velocityVariation_x;
		if (vx >= ITEM_FALLING_SPEED_X || vx <= -ITEM_FALLING_SPEED_X)
			velocityVariation_x *= -1; // đổi chiều
	}

	if (CSimon::GetInstance()->isColisionItem(this))
	{
		isEnable = false;
		CSimon::GetInstance()->colisionItem(this);
	}
	if (isTouchGround) return;

	//vector<LPGAMEOBJECT> listObject_Ground;
	//listObject_Ground.clear();
	//for (UINT i = 0; i < coObjects->size(); i++) {
	//	if (coObjects->at(i)->type == ObjectType::GROUND)
	//		listObject_Ground.push_back(coObjects->at(i));
	//}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents); // Lấy danh sách các va chạm 
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
	//RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
		l = x;
		t = y;
		r = x + width;
		b = y + height;
}

void CItem::stateChange()
{
	switch (item)
	{
	case TYPE_NONE:
		width = 0;
		height = 0;
		break;
	case BIG_HEART:
		width = BIG_HEART_BBOX;
		height = BIG_HEART_BBOX;
		break;
	case SMALL_HEART:
		width = SMALL_HEART_BBOX;
		height = SMALL_HEART_BBOX;
		break;
	case WHIP:
	case KNIFE:
	case AXE:
	case STOP_WATCH:
	case INVISIBLE:
	case HOLY_WATER:
		width = WEAPON_BBOX;
		height = WEAPON_BBOX;
		break;
	case POINT_400:
	case POINT_700:
	case POINT_1000:
	case FOOD:
	case DOUBLE_SHOT:
	case TRIPLE_SHOT:
	default:
		width = PACKAGE_BBOX;
		height = PACKAGE_BBOX;
		break;
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
		stateChange();
		isDestroyed = true;
		isDestroying = false;
		timeDisplayed = 0;
		break;
	default:
		break;
	}
}

ItemType CItem::randomItem()
{
	return ItemType(rand() % 16 + 1);
}

