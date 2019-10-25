#pragma once

#include "define.h"
#include "Game.h"
#include "GameObject.h"

#include "Simon.h"
#include "tilemap.h"

#include <map>
using namespace std;

class CScenes
{

	static CScenes * __instance; // Singleton Patern

	vector<LPGAMEOBJECT> objects; //các object của map

	vector<LPGAMEOBJECT> onCamObjects; //các object trong camera

	int currentScene = SCENE_1; // mặc định là scene 1

public:

	CScenes();
	~CScenes();

	void Update(DWORD dt);

	void Render();

	// Get, Set
	void pushObject(LPGAMEOBJECT object);
	void insertObject(LPGAMEOBJECT object);
	void clearAllObject();
	int getListObjectSize() { return objects.size(); };
	void putItem(ItemType type, float x, float y);
	int getCurrentScene() { return currentScene; };
	void setCurrentScene(int id) { currentScene = id; };
	void updateCamPos();
	static CScenes * GetInstance();


};

