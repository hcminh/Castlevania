#include "Scenes.h"
#include "Ground.h"
#include "Door.h"
#include "Stair.h"
#include "Enemy.h"
#include "Zombie.h"
#include "Fish.h"
#include "Dog.h"
#include "Bat.h"
#include "Water.h"
#include "SupportObject.h"
#include <fstream>

CScenes * CScenes::__instance = NULL;

CScenes::CScenes()
{
	camera = CCamera::GetInstance();
	simon = CSimon::GetInstance();
}

void CScenes::Update(DWORD dt)
{
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

			if (obj.second->type == ObjectType::GROUND)
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
			if (obj.second->isEnable)
			{
				obj.second->Update(dt, &grounds); //obj khác ngoài simon chỉ cần kt va chạm vs ground
			}
		}

		if (doors.size() != 0)
		{
			simon->checkColisionDoor(doors);
		}

		simon->Update(dt, &onCamObjects);
	}
	// update camera
	updateCam();
}

void CScenes::Render()
{
	CMaps::GetInstance()->Get(curentMap)->Draw(camera->getCamPos());
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

void CScenes::setStateWidth()
{
	switch (stateGame)
	{
	case STATE_1:
	case STATE_3:
	default:
		inZombiesActiveArea = false;
		startPointOfState = 0.0f;
		stateWidth = CMaps::GetInstance()->Get(curentMap)->GetMapWidth();
		break;
	case STATE_2_1:
		inZombiesActiveArea = true;
		startPointOfState = 0.0f;
		stateWidth = 3092;
		break;
	case STATE_2_2:
		inZombiesActiveArea = false;
		//startPointOfState = 3072;
		stateWidth = 4113;
		break;
	case STATE_2_3:
		inZombiesActiveArea = true;
		startPointOfState = 4100;
		stateWidth = 5632;
		break;
	}
}

void CScenes::insertObject(LPGAMEOBJECT object)
{
	this->objects.insert(make_pair(object->ID, object));
}

void CScenes::clearAllObject()
{
	objects.clear();
}

void CScenes::updateCam()
{
	setStateWidth();
	camera->update(stateWidth, startPointOfState);
}

void CScenes::changeScene(LPGAMEOBJECT obj)
{
	auto door = dynamic_cast<CDoor *> (obj);
	currentScene = door->nextScene;
	curentMap = scenes[currentScene]->mapID;
	stateGame = door->nextStateGame;
	setStateWidth();
	if(stateGame == STATESCENE::STATE_2_2) startPointOfState = 3072; //trường hợp đặc biệt do bị ảnh hưởng bởi 2 cái cửa //fix sau
	loadObjectToGrid(scenes[currentScene]->linkObjects);
	simon->SetPosition(door->newPosX, door->newPosY);
	updateCam();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
}

void CScenes::changeScene(SCENEID newScene)
{
	if (newScene == SCENEID_1)
	{
		currentScene = newScene;
		curentMap = scenes[currentScene]->mapID;
		stateGame = STATE_1;
		setStateWidth();
		startPointOfState = 0;
		simon->SetPosition(1000.0f, 300);
	}
	else if (newScene == SCENEID_2)
	{
		currentScene = newScene;
		curentMap = scenes[currentScene]->mapID;
		stateGame = STATE_2_1;
		setStateWidth();
		startPointOfState = 0;
		simon->SetPosition(2961, 100);
	}
	else if (newScene == SCENEID_3)
	{
		currentScene = newScene;
		curentMap = scenes[currentScene]->mapID;
		stateGame = STATE_3;
		setStateWidth();
		startPointOfState = 0;
		simon->SetPosition(10, 330);
	}

	loadObjectToGrid(scenes[currentScene]->linkObjects);
	updateCam();
	getObjectsFromGrid(camera->getCamPosX(), SCREEN_WIDTH);
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
	int idInGame, width, height, state;
	float x, y;
	fs >> numOfCell;
	grid->initListCells(numOfCell);
	while (!fs.eof())
	{
		fs >> cellIndex >> id >> x >> y >> idInGame >> state >> width >> height;

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
			obj = new CDoor(STATESCENE(state), width, height, x, y); //width, heght đại diện cho vị trí mới của Simon khi qua màn. state là id của next scene
			break;
		}
		case ENEMY_DOG:
		{
			obj = new CDog(x, y);
			break;
		}
		case ENEMY_ZOMBIE:
		{
			LPGAMEOBJECT zom = new CZombie(width, state);
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
			obj = new CStair(STATESTAIR(state), width, height, x, y); //đại diện cho số bậc và vị trí của bậc thang đầu tiên
			break;
		}
		case SUPPORTER:
		{
			obj = new CSupportObject(STATESP(state), width, x, y); //hidden obj để hỗ trợ auto walk qua màn //width đại diện cho khoảng cách cần đi tiếp
			break;
		}
		case WATER:
		{
			obj = new CWater(); 
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

