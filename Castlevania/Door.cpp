#include "Door.h"



CDoor::CDoor(float x, float y)
{
	type = ObjectType::DOOR;
	this->x = x;
	this->y = y;
	
}

void CDoor::Render()
{
	RenderBoundingBox();
}

void CDoor::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + DOOR_BBOX_WIDTH;
	b = y + DOOR_BBOX_HEIGHT;
}


