#pragma once
#include "GameObject.h"
#include "Scenes.h"
#include "NextStage.h"
#include "debug.h"

#define DOOR_BBOX_WIDTH  32
#define DOOR_BBOX_HEIGHT 60

class CDoor : public CGameObject
{
public:
	SCENEID nextSceneID;
	STAGEID nextStageID;
	float newPosX = 0;
	float newPosY = 0;
	CDoor(STAGEID nextStage, SCENEID nextScene, float newPosX, float newPosY, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};