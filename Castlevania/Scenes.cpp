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

	getObjectsFromGrid(CGame::GetInstance()->getCamPosX(), SCREEN_WIDTH);

	onCamObjects.clear();
	for (auto obj : objects)
		if (obj.second->isEnable && onCamera(obj.second, CGame::GetInstance()->getCamPosX()))
			onCamObjects.push_back(obj.second);

	insertObject(CSimon::GetInstance()); // để push con simon vào mảng objects sau khi đã lấy dc mảng oncamera để khỏi bị va cham với dao hoặc subweapon khác

	for (auto obj : objects)
		if (obj.second->isEnable)
			obj.second->Update(dt, &onCamObjects);
	// update camera
	updateCamPos();
}

void CScenes::Render()
{
	CMaps::GetInstance()->Get(curentMap)->Draw(CGame::GetInstance()->getCamPos());
	for (int i = 0; i < onCamObjects.size(); i++)
		onCamObjects[i]->Render();
	CSimon::GetInstance()->Render(); //render lol simon cuối cùng để nó đè lên mấy thằng kia
}

void CScenes::Add(SceneID sceneID, int mapID, string linkObjects)
{
	LPSCENE scene = new CScene(sceneID, mapID, linkObjects);
	scenes[sceneID] = scene;
}

void CScenes::insertObject(LPGAMEOBJECT object)
{
	this->objects.insert(make_pair(object->ID, object));
}

void CScenes::clearAllObject()
{
	objects.clear();
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
			CGame::GetInstance()->SetCamPos(xSimon - SCREEN_WIDTH / 2, 0);
		}
	}
}

bool CScenes::onCamera(LPGAMEOBJECT obj, int xCam)
{
	if (obj->x < xCam - 32 && (obj->x + obj->width < xCam - 32)) return false;
	if (obj->x > xCam + SCREEN_WIDTH + 32) return false;
	return true;
}

void CScenes::changeScene()
{
	if (currentScene == SceneID::SCENEID_1)
	{
		currentScene = SceneID::SCENEID_2;
		curentMap = scenes[currentScene]->mapID;
		loadObjectToGrid(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(0.0f, 300);
		CGame::GetInstance()->SetCamPos(0, 0);
		getObjectsFromGrid(CGame::GetInstance()->getCamPosX(), SCREEN_WIDTH);
	}
	else if (currentScene == SceneID::SCENEID_2)
	{
		currentScene = SceneID::SCENEID_3;
		curentMap = scenes[currentScene]->mapID;
		loadObjectToGrid(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(0.0f, 100);
		CGame::GetInstance()->SetCamPos(0, 0);
		getObjectsFromGrid(CGame::GetInstance()->getCamPosX(), SCREEN_WIDTH);
	}
	else if (currentScene == SceneID::SCENEID_3)
	{
		currentScene = SceneID::SCENEID_2;
		curentMap = scenes[currentScene]->mapID;
		loadObjectToGrid(scenes[currentScene]->linkObjects);
		CSimon::GetInstance()->SetPosition(3000.0f, 300);
		updateCamPos();
		getObjectsFromGrid(CGame::GetInstance()->getCamPosX(), SCREEN_WIDTH);
	}
}

void CScenes::changeScene(SceneID newScene)
{
	currentScene = newScene;
	curentMap = scenes[currentScene]->mapID;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
}

void CScenes::getObjectsFromGrid(int xCam, int widthCam)
{

	objects.erase(objects.begin(), objects.end());

	int indexOfFirstCell = floor(xCam / CELL_WIDTH);
	int indexOfSecondCell = floor((xCam + widthCam )/ CELL_WIDTH);

	int sizeFirstObjs = grid->cells[indexOfFirstCell]->objects.size();
	int sizeSecondObjs = grid->cells[indexOfSecondCell]->objects.size();

	for (int i = 0; i < sizeFirstObjs; i++)
	{
		insertObject(grid->cells[indexOfFirstCell]->objects[i]);
	}
	if (indexOfFirstCell != indexOfSecondCell)
		for (int i = 0; i < sizeSecondObjs; i++)
		{
			insertObject(grid->cells[indexOfSecondCell]->objects[i]);
		}

	//insertObject(CSimon::GetInstance());
}

void CScenes::loadObjectToGrid(string path)
{
	if (grid != NULL || grid != nullptr)
	{
		delete grid;
		grid = nullptr;
	}
	grid = new CGrid();
	grid->initCells(CMaps::GetInstance()->Get(curentMap)->GetMapWidth());
	clearAllObject();

	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file obecject lỗi");
		fs.close();
	}
	int id;
	int idInGame, width, height, state;
	float x, y;
	while (!fs.eof())
	{
		fs >> id >> x >> y >> idInGame >> state >> width >> height;
		switch (ObjectType(id))
		{
		case ITEM:
		{
			CItem *cItem = new CItem(ItemState(state));
			cItem->SetPosition(x, y);
			cItem->setID(idInGame);
			grid->addObjects(cItem);
			break;
		}
		case GROUND:
		{
			CGround *ground = new CGround(x, y, width, height);
			ground->setID(idInGame);
			grid->addObjects(ground);
			break;
		}
		case DOOR:
		{
			CDoor *door = new CDoor(x, y);
			door->setID(idInGame);
			grid->addObjects(door);
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

