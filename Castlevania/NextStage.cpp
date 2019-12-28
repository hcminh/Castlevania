#include "NextStage.h"

CNextStage::CNextStage(STAGEID(id), int startPointNextStage, int widthNextStage, int zombieStage, float x, float y)
{
	type = ObjectType::NEXT_STAGE;
	this->isEnable = true;
	this->nextStageID = id;
	this->startPointNextStage = startPointNextStage;
	this->widthNextStage = widthNextStage;
	this->zombieStage = zombieStage > 0 ? true : false;
	this->SetPosition(x, y);
}

void CNextStage::Render()
{
	//RenderBoundingBox();
}

void CNextStage::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + NEXTSTAGE_BBOX_WIDTH;
	b = y + NEXTSTAGE_BBOX_HEIGHT;
}


