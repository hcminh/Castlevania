#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "debug.h"
#include "Item.h"

#define ID_TEX_CANDLE	3

#define CANDLE_BIG		0
#define CANDLE_SMALL	1
#define CANDLE_BIG_ANI_DESTROYED  2

#define CANDLE_BIG_WIDTH  32
#define CANDLE_BIG_HEIGHT 64

#define CANDLE_SMALL_WIDTH  32
#define CANDLE_SMALL_HEIGHT 64

#define CANDLE_BIG_BBOX_WIDTH  32
#define CANDLE_BIG_BBOX_HEIGHT 64

#define CANDLE_SMALL_BBOX_WIDTH  32
#define CANDLE_SMALL_BBOX_HEIGHT 64

#define CANDLE_DESTROYED	999

#define TIME_DESTROY_MAX 250

class CCandle : public CGameObject
{
	DWORD timeWaited = 0;
	bool destroyed = false;
public:
	CCandle(float x, float y, ItemType item);
	~CCandle();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetState(int state);
	void LoadResources();
};