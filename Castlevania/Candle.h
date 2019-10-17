#pragma once
#include "GameObject.h"
#include "Textures.h"


#define ID_TEX_CANDLE	35


#define CANDLE_BIG		0
#define CANDLE_SMALL	1

#define CANDLE_BIG_WIDTH  32
#define CANDLE_BIG_HEIGHT 64

#define CANDLE_SMALL_WIDTH  32
#define CANDLE_SMALL_HEIGHT 64

#define CANDLE_BIG_BBOX_WIDTH  32
#define CANDLE_BIG_BBOX_HEIGHT 64

#define CANDLE_SMALL_BBOX_WIDTH  32
#define CANDLE_SMALL_BBOX_HEIGHT 64

class CCandle : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void LoadResources();
};