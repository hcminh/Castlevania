#pragma once
#include "GameObject.h"
#include "debug.h"

#define NEXTSTAGE_BBOX_WIDTH  32
#define NEXTSTAGE_BBOX_HEIGHT 60

enum STAGEID
{
	STAGE_1,
	STAGE_2_1,
	STAGE_2_2,
	STAGE_2_3,
	STAGE_3,
	STAGE_3_1,
};

class CNextStage : public CGameObject
{
public:
	int startPointNextStage;
	int widthNextStage;
	STAGEID nextStageID;
	bool zombieStage;

	CNextStage(STAGEID(id), int startPointNextStage, int widthNextStage, int zombieStage, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};