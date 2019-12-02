#pragma once
#include <vector>
#include "GameObject.h"
using namespace std;

#define CELL_WIDTH 256

class CCell
{
public:
	vector<LPGAMEOBJECT> objects;
	CCell();
};

typedef CCell * LPCELL;

class CGrid
{
public:
	vector<LPCELL> cells;
	void addObjects(LPGAMEOBJECT);
	void initCells(int mapWidth);
	CGrid();
};

