#include "Stairs.h"

CStairs * CStairs::_instance = NULL;


void CStairs::Render()
{
	for (int i = 0; i < stairs.size(); i++)
	{
		stairs[i]->RenderBoundingBox();
	}
}

CStairs * CStairs::GetInstance()
{
	if (_instance == NULL) _instance = new CStairs();
	return _instance;
}

CStair::CStair(CheckPoint cp)
{
	isEnable = true;
	checkPoint = cp;
}

void CStair::Render()
{
	RenderBoundingBox();
}

void CStair::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + STAIR_BBOX_WIDTH;
	b = t + STAIR_BBOX_HEIGHT;

	//DebugOut(L"[STAIR] CAU THANG GetBoundingBox");
}
