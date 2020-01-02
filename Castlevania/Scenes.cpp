#include "Scenes.h"
#include "Ground.h"
#include "Door.h"
#include "Stair.h"
#include "Enemy.h"
#include "Zombie.h"
#include "Fish.h"
#include "Dog.h"
#include "Bat.h"
#include "BigBat.h"
#include "Water.h"
#include "SupportObject.h"
#include <fstream>

CScenes * CScenes::__instance = NULL;

CScenes::CScenes()
{
	camera = CCamera::GetInstance();
	simon = CSimon::GetInstance();
}

void CScenes::Init()
{
	currentScene = SCENEID_1;
	curentMap = scenes[currentScene]->mapID;
	stageGame = STAGEID::STAGE_1;
	startPointStage = START_POINT_STAGE_1;
	stageWidth = STAGE_1_WIDTH;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
	simon->SetPosition(START_POINT_STAGE_1 + 10, 300);
	updateCam();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
}

void CScenes::Update(DWORD dt)
{
	if (isUsingHolyCross && (GetTickCount() - useHolyCrossTime > TIME_HOLY_CROSS))
	{
		useHolyCrossTime = 0;
		isUsingHolyCross = false;
	}
	else if (isUsingStopWatch && (GetTickCount() - useStopWatchTime > TIME_STOP_WATCH))
	{
		useStopWatchTime = 0;
		isUsingStopWatch = false;
	}


	if (!stopMovingObject)
	{

		getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);

		onCamObjects.clear();
		stairs.clear();
		grounds.clear();
		doors.clear();
		supporters.clear();

		if (inZombiesActiveArea)
		{
			for (int i = 0;i < zombies.size();i++)
			{
				insertObject(zombies[i]);
			}
		}

		for (auto obj : objects)
		{
			if (obj.second->type == ObjectType::GROUND && obj.second->state != STATE_NOT_COLLISION_WIDTH_ENEMY)
			{
				grounds.push_back(obj.second);
				if (obj.second->isEnable)
				{
					onCamObjects.push_back(obj.second);
				}
			}
			if (obj.second->type == ObjectType::WATER)
			{
				onCamObjects.push_back(obj.second);
				grounds.push_back(obj.second);
			}
			else if (obj.second->type == ObjectType::STAIR)
			{
				stairs.push_back(obj.second);
			}
			else if (obj.second->type == ObjectType::DOOR)
			{
				doors.push_back(obj.second);
			}
			else if (obj.second->isEnable)
			{
				if (obj.second->type == ObjectType::SUPPORTER)
				{
					supporters.push_back(obj.second);
				}
				onCamObjects.push_back(obj.second);
			}
		}

		for (auto obj : objects)
		{
			if (obj.second->isEnable && !isUsingStopWatch && !simon->levelUpgrade)
			{
				obj.second->Update(dt, &grounds); //obj khác ngoài simon chỉ cần kt va chạm vs ground
			}
		}

		if (doors.size() != 0)
		{
			simon->checkColisionDoor(doors);
		}

		for (auto stage : nextStages)
		{
			if (stage.second->isEnable)
				onCamObjects.push_back(stage.second);
		}

		simon->Update(dt, &onCamObjects);
	}
	// update camera
	updateCam();
}

void CScenes::Render()
{
	int alpha = 255;
	if (isUsingHolyCross) alpha = rand() % 255 + 200;
	CMaps::GetInstance()->Get(curentMap)->Draw(camera->getCamPos(), alpha);
	for (int i = 0; i < onCamObjects.size(); i++)
		onCamObjects[i]->Render();
	simon->Render(); //render simon cuối cùng để nó đè lên mấy thằng kia

	//các hiệu ứng mở cửa
	for (int i = 0; i < supporters.size(); i++)
		supporters[i]->Render();
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

void CScenes::pushObjToCam(LPGAMEOBJECT object)
{
	onCamObjects.push_back(object);
}

void CScenes::clearAllObject()
{
	objects.clear();
}

void CScenes::usingHolyCross()
{
	isUsingHolyCross = true;
	useHolyCrossTime = GetTickCount();
	for (auto obj : objects)
	{
		if (obj.second->type == ObjectType::ENEMY)
		{
			obj.second->SetState(ENEMY_STATE_DEAD);
		}
	}
}

void CScenes::usingStopWatch()
{
	isUsingStopWatch = true;
	useStopWatchTime = GetTickCount();
}

void CScenes::updateCam()
{
	camera->update(stageWidth, startPointStage);
}

void CScenes::changeScene(LPGAMEOBJECT obj)
{
	auto door = dynamic_cast<CDoor *> (obj);
	auto stage = dynamic_cast<CNextStage *> (nextStages[door->nextStageID]);
	currentScene = door->nextSceneID;
	curentMap = scenes[currentScene]->mapID;
	stageGame = door->nextStageID;
	startPointStage = stage->startPointNextStage;
	stageWidth = stage->widthNextStage;
	inZombiesActiveArea = stage->zombieStage;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
	simon->SetPosition(door->newPosX, door->newPosY);
	updateCam();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
}

void CScenes::changeScene(CNextStage * stage)
{
	curentMap = scenes[currentScene]->mapID;
	stageGame = stage->nextStageID;
	startPointStage = stage->startPointNextStage;
	stageWidth = stage->widthNextStage;
	inZombiesActiveArea = stage->zombieStage;
	loadObjectToGrid(scenes[currentScene]->linkObjects);
	updateCam();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
}

void CScenes::changeScene(STAGEID newStage)
{

	if (newStage == STAGE_1)
	{
		CDoor * door = new CDoor(newStage, SCENEID_1, 10, 300, 0, 0);
		changeScene(door);
	}
	if (newStage == STAGE_2_1)
	{
		CDoor * door = new CDoor(newStage, SCENEID_2, 10, 300, 0, 0);
		changeScene(door);
	}
	if (newStage == STAGE_2_2)
	{
		CDoor * door = new CDoor(newStage, SCENEID_2, START_POINT_STAGE_2_2 + 10, 300, 0, 0);
		changeScene(door);
	}
	if (newStage == STAGE_2_3)
	{
		CDoor * door = new CDoor(newStage, SCENEID_2, START_POINT_STAGE_2_3 + 10, 300, 0, 0);
		changeScene(door);
	}
	if (newStage == STAGE_3)
	{
		CDoor * door = new CDoor(newStage, SCENEID_3, 10, 100, 0, 0);
		changeScene(door);
	}
}

void CScenes::getObjectsFromGrid(float xCam, int widthCam)
{

	objects.erase(objects.begin(), objects.end());

	int indexOfFirstCell = floor(xCam / CELL_WIDTH);
	int indexOfLastCell = ceil((xCam + widthCam) / CELL_WIDTH);

	for (int i = indexOfFirstCell; i < indexOfLastCell; i++)
	{
		int sizeOfCell = grid->cells[i]->objects.size();
		for (int j = 0; j < sizeOfCell; j++)
		{
			insertObject(grid->cells[i]->objects[j]);
		}
	}
}

void CScenes::enableGroundHidden()
{
	for (int i = 0; i < grounds.size(); i++)
	{
		grounds[i]->isEnable = true;
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
	clearAllObject();
	zombies.clear();

	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file obecject lỗi");
		fs.close();
	}
	int id, cellIndex, numOfCell;
	int idInGame, state, param_1, param_2, param_3;
	float x, y;
	fs >> numOfCell;
	grid->initListCells(numOfCell);
	while (!fs.eof())
	{
		fs >> cellIndex >> id >> x >> y >> idInGame >> state >> param_1 >> param_2 >> param_3;

		LPGAMEOBJECT obj = NULL;
		switch (ObjectType(id))
		{
		case ITEM:
		{
			obj = new CItem(ItemType(param_2), ItemState(state), x, y);
			break;
		}
		case GROUND:
		{
			obj = new CGround(state, param_1, param_2, param_3, x, y);
			break;
		}
		case DOOR:
		{
			obj = new CDoor(STAGEID(state), SCENEID(param_1), param_2, param_3, x, y); //width, heght đại diện cho vị trí mới của Simon khi qua màn. state là id của next scene
			break;
		}
		case NEXT_STAGE:
		{
			LPGAMEOBJECT stage = new CNextStage(STAGEID(state), param_3, param_2, param_1, x, y); //height -> startpoint stage
			stage->setID(idInGame);
			nextStages.insert(make_pair(state, stage));
			break;
		}
		case ENEMY_DOG:
		{
			obj = new CDog(x, y);
			break;
		}
		case ENEMY_ZOMBIE:
		{
			LPGAMEOBJECT zom = new CZombie(param_2, state);
			zom->setID(idInGame);
			zombies.push_back(zom);
			break;
		}
		case ENEMY_FISH:
		{
			obj = new CFish();
			break;
		}
		case ENEMY_BAT:
		{
			obj = new CBat(5000, state, y); //state = nx
			break;
		}
		case STAIR:
		{
			obj = new CStair(STATESTAIR(state), param_2, param_3, x, y); //đại diện cho số bậc và vị trí của bậc thang đầu tiên
			break;
		}
		case SUPPORTER:
		{
			obj = new CSupportObject(state, param_2, x, y); //hidden obj để hỗ trợ auto walk qua màn //width đại diện cho khoảng cách cần đi tiếp
			break;
		}
		case WATER:
		{
			obj = new CWater();
			break;
		}
		case BIG_BAT:
		{
			obj = new CBigBat(x, y);
			break;
		}

		}

		if (obj != NULL)
		{
			obj->setID(idInGame);
			grid->addObject(cellIndex, obj);
		}
	}
	fs.close();

}

CScenes * CScenes::GetInstance()
{
	if (__instance == NULL) __instance = new CScenes();
	return __instance;
}

CScene::CScene(SCENEID sceneID, int mapID, string link)
{
	this->sceneID = sceneID;
	this->mapID = mapID;
	this->linkObjects = link;
}

