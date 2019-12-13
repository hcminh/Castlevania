#include "Water.h"

CWater::CWater()
{
	type = ObjectType::WATER;
	isEnable = true;
	SetPosition(0, 368);
	width = 1056;
	height = 32;
}

void CWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

}

void CWater::Render()
{
	RenderBoundingBox();
}

void CWater::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = t + height;
}


