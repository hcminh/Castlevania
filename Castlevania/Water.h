#pragma once

#include <vector>

#include "GameObject.h"

// Bubbles
#define BUBBLES_GRAVITY				0.0005f
#define BUBBLES_TIME				1000

class CBubble : public CGameObject
{
public:
	CBubble();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {}
};

typedef CBubble * LPBUBBLE;

// Vì mỗi cụm có 3 Bubble nên tạo class Bubbles để quản lí
class CBubbles
{
	DWORD startTime = 0;

public:
	vector<LPBUBBLE> bubbles;
	CBubbles(float x, float y);
	~CBubbles();

	void Update(DWORD dt);
	void Render();

	DWORD GetStartTimeRender() { return startTime; }
	void SetStartTimeRender(DWORD x) { startTime = x; }
};

class CWater : public CGameObject
{
	vector<CBubbles *> bubblesList;

public:
	CWater();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void AddBubbles(float x, float y);
};

