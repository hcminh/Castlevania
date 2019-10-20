#pragma once
#include "GameObject.h"
#include "Textures.h"

#define ID_TEX_ITEM		40	

#define ITEM_BBOX_WIDTH  16
#define ITEM_BBOX_HEIGHT 16

#define ITEM_BIG_HEART		0

class CItem : public CGameObject
{

	DWORD TimeDisplayMax; // thời gian tối đa cho phép hiển thị.
	DWORD TimeDisplayed; // Thời gian đã hiển thị.

	DWORD TimeWaited; // Thời gian đã chờ trước khi hiển thị
	DWORD TimeWaitMax; // Thời gian tối đa phải chờ trước khi hiển thị

	bool isFinish; // đã kết thúc chưa?
public:
	CItem(ItemType type);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	virtual bool isWaitingDisplay(); // đang chờ object hiển thi? - simon chưa được ăn

	void LoadResources();
};