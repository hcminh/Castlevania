#include "Door.h"



CDoor::CDoor(SCENEID nextScene, float newPosX, float newPosY, float x, float y)
{
	type = ObjectType::DOOR;
	this->nextScene = nextScene;
	if(nextScene == SCENEID::SCENEID_3)
		this->nextStateGame = STATESCENE::STATE_3;
	else if (nextScene == SCENEID::SCENEID_2)
		this->nextStateGame = STATESCENE::STATE_2_2; // set cứng tạm thời vầy để tìm cách giải quết vì lười thêm feild mới vào file txt
	this->newPosX = newPosX;
	this->newPosY = newPosY;
	this->x = x;
	this->y = y;
	isEnable = true;
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


