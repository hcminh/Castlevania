#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "define.h"

#define ID_TEX_ITEM		40	

#define BIG_HEART_BBOX  23
#define BIG_HEART_GRAVITY 0.002f
#define BIG_HEART_BBOX_HEIGHT 22

#define ITEM_BIG_HEART		0
#define ITEM_SMALL_HEART	1
#define ITEM_WHIP			2
#define ITEM_KNIFE			3

#define TIME_DELAY_ITEM		250
#define TIME_APPEAR_ITEM	10000

class CItem : public CGameObject
{
	DWORD timeDisplayed; // Thời gian đã hiển thị.

	DWORD timeWaited; // Thời gian đã chờ trước khi hiển thị

	bool isFinish; // đã kết thúc chưa?
public:
	CItem(ItemType type);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void LoadResources();
};