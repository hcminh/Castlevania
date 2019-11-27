#include "Stair.h"

CStair::CStair(CheckPoint cp)
{
	isEnable = true;
	type = ObjectType::STAIR;
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
