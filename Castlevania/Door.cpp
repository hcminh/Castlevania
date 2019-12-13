#include "Door.h"

CDoor::CDoor(STATESCENE nextState, float newPosX, float newPosY, float x, float y)
{
	type = ObjectType::DOOR;
	this->nextStateGame = nextState;
	this->newPosX = newPosX;
	this->newPosY = newPosY;
	this->x = x;
	this->y = y;
	isEnable = true;
	switch (this->nextStateGame)
	{
	case STATE_1:
		break;
	case STATE_2_1:
	case STATE_2_2:
	case STATE_2_3:
		this->nextScene = SCENEID::SCENEID_2;
		break;
	case STATE_3:
		this->nextScene = SCENEID::SCENEID_3;
		break;
	}
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


