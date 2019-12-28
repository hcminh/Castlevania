#pragma once
#include "GameObject.h"
#include "Scenes.h"
#include "debug.h"

#define SPOBJ_BBOX_WIDTH  32
#define SPOBJ_DOOR_1_TO_2_BBOX_HEIGHT 96
#define SPOBJ_STOP_CAM_BBOX_HEIGHT 360

#define AUTOWALK_TO_DOOR_1		0
#define STOP_CAM				1
#define CONECT_SCENE			2	
#define STOP_CAM_2				3
#define TURN_OF_CAM_UPDATE		4
#define DOOR_1_TO_2				5
#define DOOR_2_TO_2_2			6
#define DOOR_2_2_TO_2_3			7


class CSupportObject : public CGameObject
{
public:
	float distanceAutoWalk = 0;
	CSupportObject(int stateSp, float distance, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};