#include "Scenes.h"
#include "Ground.h"
#include "Door.h"
#include "Stair.h"
#include "Enemy.h"
#include "Zombie.h"
#include "Fish.h"
#include "Dog.h"
#include "SupportObject.h"

CScenes * CScenes::__instance = NULL;

CScenes::CScenes()
{
	camera = CCamera::GetInstance();
	simon = CSimon::GetInstance();
}

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

	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);

	onCamObjects.clear();
	stairs.clear();

	for (auto obj : objects)
		if (obj.second->isEnable && camera->onCamera(obj.second->x, obj.second->x + obj.second->width))
		{
			if (obj.second->type == ObjectType::STAIR)
				stairs.push_back(obj.second);
			else //nhớ bật cái này lên để ko cho cái stair vào cam mắc công va chạm vs enemy
				onCamObjects.push_back(obj.second);
		}

	for (auto obj : onCamObjects)
		if (obj->isEnable)
			obj->Update(dt, &onCamObjects);
	//for (auto obj : objects)
	//	if (obj.second->isEnable)
	//		obj.second->Update(dt, &onCamObjects);
	simon->Update(dt, &onCamObjects);
	// update camera
	updateCamPos();
}

void CScenes::Render()
{
	CMaps::GetInstance()->Get(curentMap)->Draw(camera->getCamPos());
	for (int i = 0; i < onCamObjects.size(); i++)
		onCamObjects[i]->Render();
	simon->Render(); //render simon cuối cùng để nó đè lên mấy thằng kia
}

void CScenes::Add(SCENEID sceneID, int mapID, string linkObjects)
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
	float xSimon = simon->x + SIMON_SPRITE_WIDTH;
	int mapWidth = CMaps::GetInstance()->Get(curentMap)->GetMapWidth();
	if (camera->stopMoving) { mapWidth = 3092; return; }

	if (xSimon - startPointOfState > SCREEN_WIDTH / 2 &&
		xSimon + SCREEN_WIDTH / 2 < mapWidth)
	{
			camera->SetCamPos(xSimon - SCREEN_WIDTH / 2, 0);
	}
	else if(xSimon - startPointOfState < SCREEN_WIDTH / 2)
		camera->SetCamPos(startPointOfState, 0);
	else if(xSimon > SCREEN_WIDTH / 2) camera->SetCamPos(mapWidth - SCREEN_WIDTH - 4, 0);
}

void CScenes::changeScene(LPGAMEOBJECT obj)
{
	auto door = dynamic_cast<CDoor *> (obj);

	currentScene = door->nextScene;
	curentMap = scenes[currentScene]->mapID;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
	simon->SetPosition(door->newPosX, door->newPosY);
	startPointOfState = 0;
	updateCamPos();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
}

void CScenes::changeScene(SCENEID newScene)
{
	currentScene = newScene;
	curentMap = scenes[currentScene]->mapID;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
}

void CScenes::getObjectsFromGrid(int xCam, int widthCam)
{

	objects.erase(objects.begin(), objects.end());

	int indexOfFirstCell = floor(xCam / CELL_WIDTH);
	int indexOfLastCell = floor((xCam + widthCam) / CELL_WIDTH);

	for (int i = indexOfFirstCell; i <= indexOfLastCell; i++)
	{
		int sizeOfCell = grid->cells[i]->objects.size();
		for (int j = 0; j < sizeOfCell; j++)
		{
			insertObject(grid->cells[i]->objects[j]);
		}
	}
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

		LPGAMEOBJECT obj = NULL;
		switch (ObjectType(id))
		{
		case ITEM:
		{
			obj = new CItem(x, y, ItemState(state));
			break;
		}
		case GROUND:
		{
			obj = new CGround(x, y, width, height);
			break;
		}
		case DOOR:
		{
			obj = new CDoor(SCENEID(state), width, height, x, y); //width, heght đại diện cho vị trí mới của Simon khi qua màn. state là id của next scene
			break;
		}
		case ENEMY:
		{
			obj = new CDog(x, y);
			break;
		}
		case STAIR:
		{
			obj = new CStair(STATESTAIR(state), width, height, x, y); //đại diện cho số bậc và vị trí của bậc thang đầu tiên
			break;
		}
		case SUPPORTER:
		{
			obj = new CSupportObject(STATESP(state), width, x, y); //hidden obj để hỗ trợ auto walk qua màn //width đại diện cho khoảng cách cần đi tiếp
			break;
		}

		}

		obj->setID(idInGame);
		grid->addObjects(obj);
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

CScene::CScene(SCENEID sceneID, int mapID, string link)
{
	this->sceneID = sceneID;
	this->mapID = mapID;
	this->linkObjects = link;
}

