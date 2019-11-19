#pragma once

#include <map>
#include <unordered_map>

#include "define.h"
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "Simon.h"
#include "Zombie.h"
#include "tilemap.h"
#include "Grid.h"

#define MAX_TIME_STOP_WATCH 5000
using namespace std;

enum SceneID
{
	SCENEID_0,
	SCENEID_1,
	SCENEID_2,
	SCENEID_3,
};

class CScene
{
public:

	string linkObjects;
	SceneID sceneID = SceneID::SCENEID_1; // mặc định là scene 1
	int mapID;
	CScene(SceneID sceneID, int mapID, string link);

};

typedef CScene * LPSCENE;

class CScenes
{
	static CScenes * __instance; // Singleton Patern
	unordered_map<int, LPGAMEOBJECT> objects; //các object của map
	vector<LPGAMEOBJECT> onCamObjects; //các object trong camera
	unordered_map<SceneID, LPSCENE> scenes;
	SceneID currentScene;
	int curentMap;
	DWORD accutime = 0;
	CGrid *grid;
	LPCAMERA camera; // lưu địa chỉ cam lại để khỏi phải gọi getinstance()
	LPSIMON simon; //lưu luôn con simon cho lẹ
public:
	bool isStopWatchInUse = false;

	CScenes();
	void Update(DWORD dt);
	void Render();
	void Add(SceneID sceneID, int mapID, string linkObjects);
	LPSCENE getCurrentScene() { return scenes[SceneID(currentScene)]; };
	static CScenes * GetInstance();
	// Get, Set
	void insertObject(LPGAMEOBJECT object);
	void clearAllObject();
	void updateCamPos();
	void changeScene();
	void changeScene(SceneID newScene);
	void loadObjectToGrid(string path);
	void getObjectsFromGrid(int xCam, int widthCam);
	void stopObject();
	int getObjectsSize() { return objects.size(); };

};

