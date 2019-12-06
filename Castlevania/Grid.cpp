#include "Grid.h"



void CGrid::addObjects(LPGAMEOBJECT obj)
{
	int firstCellOfObj = floor(obj->x / CELL_WIDTH) >= 0 ? floor(obj->x / CELL_WIDTH) : 0;
	int lastCellOfObj = floor((obj->x + obj->width) / CELL_WIDTH) >= 0 ? floor((obj->x + obj->width) / CELL_WIDTH) : 0;

	for (int i = firstCellOfObj; i <= lastCellOfObj; i++)
	{
		cells[i]->objects.push_back(obj);
	}
}
void CGrid::addObject(int cellIndex, LPGAMEOBJECT obj)
{
		cells[cellIndex]->objects.push_back(obj);
}

void CGrid::initCells(int mapWidth)
{
	int numOfCells = ceil(mapWidth / CELL_WIDTH);
	for (int i = 0; i <= numOfCells; i++)
	{
		CCell *cell = new CCell();
		cells.push_back(cell);
	}
}

void CGrid::initListCells(int numOfCells)
{
	for (int i = 0; i < numOfCells; i++)
	{
		CCell *cell = new CCell();
		cells.push_back(cell);
	}
}

CGrid::CGrid()
{
}



CCell::CCell()
{
}
