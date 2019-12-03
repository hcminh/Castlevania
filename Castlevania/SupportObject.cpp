#include "SupportObject.h"

CSupportObject::CSupportObject(STATESP stateSp, float distance, float x, float y)
{
	type = ObjectType::SUPPORTER;
	this->stateSp = stateSp;
	this->distanceAutoWalk = distance;
	this->x = x;
	this->y = y;
	isEnable = true;
	//switch (this->stateSp)
	//{
	//case DOOR_1_TO_2:
	//	AddAnimation(319); // hình cửa ở scene 1
	//	break;
	//default:
	//	break;
	//}
}

void CSupportObject::Render()
{
	//if (isActive)
	//{
	//	animations[0]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}
	//RenderBoundingBox();
}

void CSupportObject::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + SPOBJ_BBOX_WIDTH;
	if (stateSp == DOOR_1_TO_2 || stateSp == CONECT_SCENE_2 || stateSp == TURN_OF_CAM_UPDATE)
	{
		b = y + SPOBJ_DOOR_1_TO_2_BBOX_HEIGHT;
	}
	else if(stateSp == STOP_CAM || stateSp == STOP_CAM_2)
	{
		b = y + SPOBJ_STOP_CAM_BBOX_HEIGHT;
	}
}


