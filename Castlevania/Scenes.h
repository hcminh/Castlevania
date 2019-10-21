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

public:

	CScenes();
	~CScenes();

	void Update(DWORD dt);

	void Render();

	// Get, Set
	void pushObject(LPGAMEOBJECT objects);
	int getListObjectSize() { return objects.size(); };
	void putItem(ItemType type, float x, float y);

	void updateCamPos();
	static CScenes * GetInstance();


};

