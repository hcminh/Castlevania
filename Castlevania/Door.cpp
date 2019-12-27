#include "Door.h"

CDoor::CDoor(STAGEID nextStageID, SCENEID nextSceneID,float newPosX, float newPosY, float x, float y)
{
	type = ObjectType::DOOR;
	this->nextStageID = nextStageID;
	this->nextSceneID = nextSceneID;
	this->newPosX = newPosX;
	this->newPosY = newPosY;
	this->x = x;
	this->y = y;
	isEnable = true;
}

void CDoor::Render()
{
	//RenderBoundingBox();
}

void CDoor::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + DOOR_BBOX_WIDTH;
	b = y + DOOR_BBOX_HEIGHT;
}


