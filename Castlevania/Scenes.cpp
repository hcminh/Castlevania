﻿#include "Scenes.h"

CScenes * CScenes::__instance = NULL;

CScenes::CScenes()
{
}

CScenes::~CScenes()
{
}

void CScenes::Update(DWORD dt)
{	
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// update camera
	updateCamPos();
}

void CScenes::Render()
{
	CMaps::GetInstance()->Get(SCENE_1)->Draw(CGame::GetInstance()->getCamPos());
	for (int i = 1; i < objects.size(); i++)
		objects[i]->Render();

	objects[0]->Render(); //render lol simon cuối cùng để nó đè lên mấy thằng kia
}

void CScenes::pushObject(LPGAMEOBJECT object)
{
	this->objects.push_back(object);
}

void CScenes::insertObject(LPGAMEOBJECT object)
{
	this->objects.insert(objects.begin(), object);
}

void CScenes::putItem(ItemType type, float x, float y)
{
	CItem *item = new CItem(type);
	item->SetPosition(x, y);
	pushObject(item);
}

void CScenes::updateCamPos()
{
	float xSimon = CSimon::GetInstance()->x + SIMON_SPRITE_WIDTH;
	if (xSimon > SCREEN_WIDTH / 2 &&
		xSimon + SCREEN_WIDTH / 2 < CMaps::GetInstance()->Get(SCENE_1)->GetMapWidth())
	{

		CMap * map = CMaps::GetInstance()->Get(SCENE_1);

		if (xSimon >= MAP_1_MIN_COL * TILE_WIDTH + (SCREEN_WIDTH / 2 - 16) &&
			xSimon <= MAP_1_MAX_COL * TILE_WIDTH - (SCREEN_WIDTH / 2 - 16))
		{
			CGame::GetInstance()->SetCamPos(xSimon - SCREEN_WIDTH / 2, 0);
		}
	}
}

CScenes * CScenes::GetInstance()
{
	if (__instance == NULL) __instance = new CScenes();
	return __instance;
}



