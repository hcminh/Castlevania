#include "Water.h"


CBubble::CBubble()
{
	AddAnimation(270);
}

void CBubble::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	vy += BUBBLES_GRAVITY * dt;

	x += dx;
	y += dy;
}

void CBubble::Render()
{
	animations[0]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CBubbles::CBubbles(float x, float y)
{
	// Khởi tạo 3 bubble

	CBubble * bubble;

	for (int i = 0; i < 3; i++)
	{
		bubble = new CBubble();
		bubble->SetPosition(x, y);

		float vx = (float)(-200 + rand() % 300) / 1000;
		float vy = (float)(-200 + rand() % 300) / 1000;
		bubble->SetSpeed(vx, vy);

		bubbles.push_back(bubble);
	}
}

CBubbles::~CBubbles()
{
	for (UINT i = 0; i < 3; i++)
		delete(bubbles[i]);

	bubbles.clear();
}

void CBubbles::Update(DWORD dt)
{
	for (auto bubble : bubbles)
	{
		bubble->Update(dt);
	}
}

void CBubbles::Render()
{
	for (auto bubble : bubbles)
	{
		bubble->Render();
	}
}

CWater::CWater()
{
	type = ObjectType::WATER;
	isEnable = true;
	SetPosition(0, 380);
	width = 1056;
	height = 32;
}

void CWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	DWORD now = GetTickCount();

	for (UINT i = 0; i < bubblesList.size(); i++)
	{
		if (now - bubblesList[i]->GetStartTimeRender() > BUBBLES_TIME)
		{
			bubblesList.erase(bubblesList.begin() + i);
			i--;
		}
		else
			bubblesList[i]->Update(dt);
	}
}

void CWater::Render()
{
	for (auto bubbles : bubblesList)
		bubbles->Render();
	//RenderBoundingBox();
}

void CWater::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = t + height;
}

void CWater::AddBubbles(float x, float y)
{
	CBubbles * bubbles = new CBubbles(x, y);
	bubbles->SetStartTimeRender(GetTickCount());

	bubblesList.push_back(bubbles);
}

