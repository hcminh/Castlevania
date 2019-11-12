#include "Scenes.h"
#include "Ground.h"
#include "Door.h"
#include "Enemy.h"
#include "Stairs.h"

CScenes * CScenes::__instance = NULL;


void CScenes::Update(DWORD dt)
{
	if (isStopWatchInUse)
	{
		accutime += dt;
		if (accutime >= MAX_TIME_STOP_WATCH)
		{
			CSimon::GetInstance()->subWeapon->isStopWatch = false;
			CSimon::GetInstance()->isUseSubWeapon = false;
			CSimon::GetInstance()->subWeapon->isFlying = false;
			isStopWatchInUse = false;
		}
	}
	onCamObjects.clear();
	for (int i = 1; i < objects.size(); i++)
		if (objects[i]->isEnable)
			onCamObjects.push_back(objects[i]);

	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->isEnable)
			objects[i]->Update(dt, &onCamObjects);
	// update camera
	updateCamPos();
}

void CScenes::Render()
{
	CMaps::GetInstance()->Get(curentMap)->Draw(CGame::GetInstance()->getCamPos());
	for (int i = 0; i < onCamObjects.size(); i++)
		onCamObjects[i]->Render();
	objects[0]->Render(); //render lol simon cuối cùng để nó đè lên mấy thằng kia

	//render stair ra nhìn thôi, mốt xóa
	CStairs::GetInstance()->Render();
}

void CScenes::Add(SceneID sceneID, int mapID, string linkObjects)
{
	LPSCENE scene = new CScene(sceneID, mapID, linkObjects);
	scenes[sceneID] = scene;
}

void CScenes::pushObject(LPGAMEOBJECT object)
{
	this->objects.push_back(object);
}

void CScenes::insertObject(LPGAMEOBJECT object)
{
	this->objects.insert(objects.begin(), object);
}

void CScenes::clearAllObject()
{
	objects.clear();
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
	int mapWidth = CMaps::GetInstance()->Get(curentMap)->GetMapWidth();

	if (xSimon > SCREEN_WIDTH / 2 &&
		xSimon + SCREEN_WIDTH / 2 < mapWidth)
	{
		if (xSimon >= SCREEN_WIDTH / 2 &&
			xSimon <= mapWidth - (SCREEN_WIDTH / 2))
		{
			CGame::GetInstance()->SetCamPos(xSimon - SCREEN_WIDTH / 2 , 0); 
		}
	}
}

void CScenes::changeScene()
{
	if (currentScene == SceneID::SCENEID_1)
	{
		currentScene = SceneID::SCENEID_2;
		curentMap = scenes[currentScene]->mapID;
		loadObject(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(0.0f, 300);
		CGame::GetInstance()->SetCamPos(0, 0); 
	}
	else if (currentScene == SceneID::SCENEID_2)
	{
		currentScene = SceneID::SCENEID_3;
		curentMap = scenes[currentScene]->mapID;
		loadObject(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(0.0f, 100);
		CGame::GetInstance()->SetCamPos(0, 0);
	}
	else if (currentScene == SceneID::SCENEID_3)
	{
		currentScene = SceneID::SCENEID_2;
		curentMap = scenes[currentScene]->mapID;
		loadObject(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(3000.0f, 300);
		updateCamPos();
	}
}

void CScenes::changeScene(SceneID newScene)
{
	currentScene = newScene;
	curentMap = scenes[currentScene]->mapID;
	loadObject(scenes[currentScene]->linkObjects);
}

void CScenes::loadObject(string path)
{
	clearAllObject();
	//nhét con simon vào đầu mảng cho dễ xử lý 
	insertObject(CSimon::GetInstance());

	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file obecject lỗi");
		fs.close();
	}
	int id;
	int item, width, height, state;
	float x, y;
	while (!fs.eof())
	{
		fs >> id >> x >> y >> item >> state >> width >> height;
		switch (ObjectType(id))
		{
		case ITEM:
		{
			CItem *cItem = new CItem(ItemType(item), ItemState(state));
			cItem->SetPosition(x, y);
			pushObject(cItem);
			break;
		}
		case GROUND:
		{
			CGround *ground = new CGround(x, y, width, height);
			pushObject(ground);
			break;
		}
		case DOOR:
		{
			CDoor *door = new CDoor(x, y);
			pushObject(door);
			break;
		}
		}
	}
	fs.close();
}

CScenes * CScenes::GetInstance()
{
	if (__instance == NULL) __instance = new CScenes();
	return __instance;
}

void CScenes::stopObject()
{
	isStopWatchInUse = true;
	accutime = 0;
}

CScene::CScene(SceneID sceneID, int mapID, string link)
{
	this->sceneID = sceneID;
	this->mapID = mapID;
	this->linkObjects = link;
}
