#include "tilemap.h"
#include "Define.h"
#include <string>     // std::string, std::to_string

CMaps * CMaps::_instance = NULL;

CMap::CMap(int ID, LPCWSTR dataPath)
{
	this->ID = ID;

	LoadMap(dataPath);
}

void CMap::LoadMap(LPCWSTR dataPath)
{
	CSprites * sprites = CSprites::GetInstance();
	fstream fs;
	fs.open(dataPath, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] CMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}
	string line;
	getline(fs, line);
	stringstream ss(line);
	ss >> this->width >> this->height;
	while (!fs.eof())
	{
		getline(fs, line);

		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			int idTile = ID + n;
			spriteline.push_back(sprites->Get(idTile));
		}

		tilemap.push_back(spriteline);
	}

	fs.close();
}

void CMap::Draw(D3DXVECTOR3 camPosition)
{
	int startCol = (int)camPosition.x / 32;
	int endCol = startCol + SCREEN_WIDTH / 32;
	int numOfRow = tilemap.size();
	for (int i = 0; i < numOfRow; i++)
	{
		for (int j = startCol; j <= endCol; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm Game::Draw() có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			float x = TILE_WIDTH* (j - startCol) + camPosition.x - (int)camPosition.x % 32;
			float y = TILE_HEIGHT* i + LAYOUT_HEIGHT;

			tilemap[i][j]->Draw(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CMaps::Add(LPCWSTR dataPath, int ID)
{
	LPTILEMAP tilemap = new CMap(ID, dataPath);
	tilemaps[ID] = tilemap;
}

CMaps * CMaps::GetInstance()
{
	if (_instance == NULL) _instance = new CMaps();
	return _instance;
}
