#pragma once

#include <map>
#include <unordered_map>

#include "define.h"
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "NextStage.h"
#include "Simon.h"
#include "GameMaps.h"
#include "Grid.h"

#define TIME_STOP_WATCH 2000
#define TIME_HOLY_CROSS 1000

#define START_POINT_STAGE_1		 0.0f
#define START_POINT_STAGE_2_1	 0.0f
#define START_POINT_STAGE_2_2	 3072
#define START_POINT_STAGE_2_3	 4100
#define START_POINT_STAGE_3		 0.0f

#define STAGE_1_WIDTH		 1536 
#define STAGE_2_1_WIDTH		 3092
#define STAGE_2_2_WIDTH		 4113
#define STAGE_2_3_WIDTH		 5632
#define STAGE_3_WIDTH		 0

using namespace std;

enum SCENEID
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
	SCENEID sceneID = SCENEID::SCENEID_1; // mặc định là scene 1
	int mapID;
	CScene(SCENEID sceneID, int mapID, string link);

};

typedef CScene * LPSCENE;

class CScenes
{
	static CScenes * __instance; // Singleton Patern
	unordered_map<int, LPGAMEOBJECT> objects; //các object của map
	unordered_map<int, LPGAMEOBJECT> nextStages; //mảng quản lý các thông số của stage
	vector<LPGAMEOBJECT> onCamObjects; //các object trong camera
	//vector<LPGAMEOBJECT> specialist; //cacs obj dac biet
	vector<LPGAMEOBJECT> zombies; // quản lý zombie
	vector<LPGAMEOBJECT> grounds; // quản lý đất, vì zombie chỉ xét nó va cham vs đất thôi
	vector<LPGAMEOBJECT> doors; // quản lý đất, vì zombie chỉ xét nó va cham vs đất thôi
	vector<LPGAMEOBJECT> supporters; // list obj hỗ trợ game để render
	unordered_map<SCENEID, LPSCENE> scenes;
	SCENEID currentScene;
	int curentMap;
	DWORD accutime = 0;
	CGrid *grid;
	LPCAMERA camera; // lưu địa chỉ cam lại để khỏi phải gọi getinstance()
	LPSIMON simon; //lưu luôn con simon cho lẹ
public:
	bool isUsingStopWatch = false;
	DWORD useStopWatchTime = false;
	bool stopMovingObject = false;
	DWORD useHolyCrossTime = 0;
	bool isUsingHolyCross = false;
	bool inZombiesActiveArea = false;
	float startPointStage = 0;
	STAGEID stageGame;
	float stageWidth;
	vector<LPGAMEOBJECT> stairs; //các cầu thang trong camera
	CScenes();
	void Init();
	void Update(DWORD dt);
	void Render();
	void Add(SCENEID sceneID, int mapID, string linkObjects);
	LPSCENE getCurrentScene() { return scenes[SCENEID(currentScene)]; };
	static CScenes * GetInstance();
	// Get, Set
	void insertObject(LPGAMEOBJECT object);
	void pushObjToCam(LPGAMEOBJECT object);
	void clearAllObject();
	void usingHolyCross();
	void usingStopWatch();
	void updateCam();
	void changeScene(LPGAMEOBJECT obj);
	void changeScene(CNextStage *stage);
	void changeScene(STAGEID newStage);
	void loadObjectToGrid(string path);
	void getObjectsFromGrid(float xCam, int widthCam);
	int getObjectsSize() { return objects.size(); };
	void enableGroundHidden();

};

