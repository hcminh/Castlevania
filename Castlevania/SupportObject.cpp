﻿#include "SupportObject.h"

CSupportObject::CSupportObject(int stateSp, float distance, float x, float y)
{
	type = ObjectType::SUPPORTER;
	this->state = stateSp;
	this->distanceAutoWalk = distance;
	this->x = x;
	this->y = y;
	isEnable = true;
	switch (this->state)
	{
	case DOOR_1_TO_2:
		AddAnimation(319); // hình cửa ở scene 1
		break;
	case DOOR_2_TO_2_2:
	case DOOR_2_2_TO_2_3:
		AddAnimation(321); // hình cửa ở scene 2
		AddAnimation(320); //ANI
		break;
	default:
		break;
	}
}

void CSupportObject::Render()
{
	if (animations.size() != 0)
	{
		int ani = 0;
		if ((state == DOOR_2_TO_2_2 || state == DOOR_2_2_TO_2_3) && (CCamera::GetInstance()->autoMoving || (CSimon::GetInstance()->isAutoWalk2D && !CSimon::GetInstance()->isOnStair)))
			ani = 1;
		animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//RenderBoundingBox();
}

void CSupportObject::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	if (state == AUTOWALK_TO_DOOR_1 || state == CONECT_SCENE || state == TURN_OF_CAM_UPDATE)
	{
		r = x + SPOBJ_BBOX_WIDTH;
		b = y + SPOBJ_DOOR_1_TO_2_BBOX_HEIGHT;
	}
	else if(state == STOP_CAM || state == STOP_CAM_2)
	{
		r = x + SPOBJ_BBOX_WIDTH;
		b = y + SPOBJ_STOP_CAM_BBOX_HEIGHT;
	}
	else if(state == DOOR_1_TO_2)
	{
		r = x + 100;
		b = y + SPOBJ_STOP_CAM_BBOX_HEIGHT;
	}
	else
	{
		r = x + 15;
		b = y + SPOBJ_DOOR_1_TO_2_BBOX_HEIGHT;
	}
}


