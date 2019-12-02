#include "Stair.h"

CStair::CStair(STATESTAIR st, int ladders, int firstLadderPosX, float x, float y)
{
	this->isEnable = true;
	this->stateStair = st;
	this->ladders = ladders;
	this->firstLadderPosX = firstLadderPosX;
	this->type = ObjectType::STAIR;
	this->SetPosition(x, y);
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
