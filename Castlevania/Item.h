#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "define.h"

#define BIG_HEART_BBOX  23
#define BIG_HEART_GRAVITY 0.002f

#define CANDLE_BIG_BBOX_WIDTH  32
#define CANDLE_BIG_BBOX_HEIGHT 64



#define ITEM_BIG_CANDLE			0
#define ITEM_SMALL_CANDLE		1
#define ITEM_CANDLE_DESTROYED	2
#define ITEM_BIG_HEART			3
#define ITEM_SMALL_HEART		4
#define ITEM_WHIP				5
#define ITEM_KNIFE				6


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

class CItem : public CGameObject
{
	DWORD timeDisplayed; // Thời gian đã hiển thị.

	DWORD timeWaited; // Thời gian đã chờ trước khi hiển thị
	ItemState itemState;
	bool isDestroyed = false;
	bool isDestroying = false;
	bool isTouchGround = false;
public:
	CItem(ItemType type, ItemState itemState = ItemState::STATE_NONE);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetState(int state);
	void LoadResources();
};