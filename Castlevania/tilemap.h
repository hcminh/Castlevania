#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Sprites.h"
#include "Debug.h"
#include "Textures.h"

#define TILEMAP_TRANSPARENT_COLOR		D3DCOLOR_XRGB(5,5,5)
using namespace std;

class CMap
{
	vector<vector<LPSPRITE>> tilemap;

	int width;					// chiều rộng của map
	int height;					// chiều cao của map	
	int ID;							// ID map để gọi cho dễ

public:
	CMap() {}
	CMap(int ID, LPCWSTR dataPath);
	void LoadMap(LPCWSTR dataPath);					// load ma trận map 
	void Draw(D3DXVECTOR3 camPosition, int alpha);	// vẽ tất cả các tile nằm trong camera
	int GetMapWidth();
};

typedef CMap * LPTILEMAP;


class CMaps
{
	static CMaps * _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	void Add(LPCWSTR filePath_data, int ID);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static CMaps * GetInstance();
};
