#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "define.h"

#define BIG_HEART_BBOX  23
#define SMALL_HEART_BBOX  15
#define BIG_HEART_GRAVITY 0.2f
#define SMALL_HEART_GRAVITY 0.0001f

#define CANDLE_BIG_BBOX_WIDTH		 32
#define CANDLE_BIG_BBOX_HEIGHT		 64
#define CANDLE_SMALL_BBOX_WIDTH		 15
#define CANDLE_SMALL_BBOX_HEIGHT     40
#define WEAPON_BBOX					 32
#define PACKAGE_BBOX				 28

#define ITEM_ANI_STATE			0
#define ITEM_ANI_ITEM			1
#define ITEM_ANI_DETROY			2

#define ITEM_BIG_CANDLE			0
#define ITEM_SMALL_CANDLE		1
#define ITEM_CANDLE_DESTROYED	2
#define ITEM_BIG_HEART			3
#define ITEM_SMALL_HEART		4
#define ITEM_WHIP				5
#define ITEM_KNIFE				6

#define ITEM_FALLING_SPEED_X			0.15f
#define ITEM_FALLING_SPEED_X_VARIATION	0.007f

#define CANDLE_STATE_DESTROYING		999
#define CANDLE_STATE_DESTROYED		-999

#define TIME_DELAY_ITEM		350
#define TIME_APPEAR_ITEM	5000

enum ItemState
{
	STATE_NONE,
	STATE_BIG_CANDLE,
	STATE_SMALL_CANDLE,
	STATE_BRICK,
	STATE_WALL
};

enum ItemType
{
	TYPE_NONE,
	BIG_HEART,
	SMALL_HEART,
	WHIP,
	KNIFE,
	STOP_WATCH,
	AXE,
	HOLY_WATER,
	HOLY_CROSS,
	INVISIBLE,
	FOOD,
	POINT_400,
	POINT_700,
	POINT_1000,
	DOUBLE_SHOT,
	TRIPLE_SHOT
};

class CItem : public CGameObject
{
	DWORD timeDisplayed; // Thời gian đã hiển thị.

	DWORD timeWaited; // Thời gian đã chờ trước khi hiển thị
	bool isDestroyed = false;
	bool isDestroying = false;
	bool isTouchGround = false;
	float velocityVariation_x;

public:
	ItemState itemState;
	ItemType item;
	CItem(ItemState itemState = ItemState::STATE_NONE);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void stateChange();
	void SetState(int state);
	ItemType randomItem();
};
