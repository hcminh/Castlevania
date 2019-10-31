#pragma once

#include "define.h"
#include "Game.h"
#include "GameObject.h"

#include "Simon.h"
#include "tilemap.h"
#include <map>

#define MAX_TIME_STOP_WATCH 5000
using namespace std;


class CScenes
{
	static CScenes * __instance; // Singleton Patern

	vector<LPGAMEOBJECT> objects; //các object của map

	vector<LPGAMEOBJECT> onCamObjects; //các object trong camera

	int currentScene = SCENE_1; // mặc định là scene 1
	DWORD accutime = 0;
public:
	bool isStopWatchInUse = false;
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
	void changeScene();
	void loadObject(string path);
	static CScenes * GetInstance();
	void stopObject();

};

