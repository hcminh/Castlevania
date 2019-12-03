#pragma once
#include "GameObject.h"
#include "Scenes.h"
#include "debug.h"

#define SPOBJ_BBOX_WIDTH  32
#define SPOBJ_DOOR_1_TO_2_BBOX_HEIGHT 60
#define SPOBJ_STOP_CAM_BBOX_HEIGHT 360

enum STATESP
{
	DOOR_1_TO_2,
	STOP_CAM, // ko cập nhật cam ở cuối state 2
	CONECT_SCENE_2,
	STOP_CAM_2, // ko cập nhật cam ở đầu state 3
	TURN_OF_CAM_UPDATE, // TẮT MẤY CÁI LIÊN QUAN ĐẾN CHUYỂN CẢNH
};

class CSupportObject : public CGameObject
{
public:
	STATESP stateSp;
	float distanceAutoWalk = 0;
	CSupportObject(STATESP stateSp, float distance, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};