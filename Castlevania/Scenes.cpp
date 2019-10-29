#include "Scenes.h"
#include "Ground.h"
#include "Door.h"

CScenes * CScenes::__instance = NULL;

CScenes::CScenes()
{
}

CScenes::~CScenes()
{
}

void CScenes::Update(DWORD dt)
{
	//vector<LPGAMEOBJECT> coObjects;
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
	CMaps::GetInstance()->Get(currentScene)->Draw(CGame::GetInstance()->getCamPos());
	for (int i = 0; i < onCamObjects.size(); i++)
		onCamObjects[i]->Render();
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
	int mapWidth = CMaps::GetInstance()->Get(currentScene)->GetMapWidth();

	if (xSimon > SCREEN_WIDTH / 2 &&
		xSimon + SCREEN_WIDTH / 2 < mapWidth)
	{
		if (xSimon >= MAP_MIN_COL * TILE_WIDTH + (SCREEN_WIDTH / 2) &&
			xSimon <= mapWidth * TILE_WIDTH - (SCREEN_WIDTH / 2))
		{
			CGame::GetInstance()->SetCamPos(xSimon - SCREEN_WIDTH / 2 , 0); //-16 để dấu ô vuông cuối đi 
		}
	}
}

void CScenes::changeScene()
{
	if (currentScene == SCENE_1)
	{
		currentScene = SCENE_2;
		loadObject("textures\\map\\scene2-objects.txt");
		CSimon::GetInstance()->SetPosition(0.0f, 300);
		CGame::GetInstance()->SetCamPos(0, 0); 
	}
	else if (currentScene == SCENE_2)
	{
		currentScene = SCENE_3;
		loadObject("textures\\map\\scene3-objects.txt");
		CSimon::GetInstance()->SetPosition(0.0f, 100);
		CGame::GetInstance()->SetCamPos(0, 0);
	}
	else if (currentScene == SCENE_3)
	{
		currentScene = SCENE_2;
		loadObject("textures\\map\\scene2-objects.txt");
		CSimon::GetInstance()->SetPosition(3000.0f, 300);
		updateCamPos();
	}
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
		switch (id)
		{
		case ID_ITEM:
		{
			CItem *cItem = new CItem(ItemType(item), ItemState(state));
			cItem->SetPosition(x, y);
			pushObject(cItem);
			break;
		}
		case ID_GROUND:
		{
			CGround *ground = new CGround(x, y, width, height);
			pushObject(ground);
			break;
		}
		case ID_DOOR:
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



