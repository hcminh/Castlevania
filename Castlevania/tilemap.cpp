#include "tilemap.h"
#include "Define.h"

CMaps * CMaps::_instance = NULL;

CMap::CMap(int ID, LPCWSTR texPath, LPCWSTR dataPath, int width, int height)
{
	this->ID = ID;
	this->width = width;
	this->height = height;

	LoadResources(texPath);
	LoadMap(dataPath);
}

void CMap::LoadResources(LPCWSTR texPath)
{
	CSprites * sprites = CSprites::GetInstance();
	CTextures * texture = CTextures::GetInstance();

	texture->Add(ID, texPath, TILEMAP_TRANSPARENT_COLOR);

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);

	// lấy thông tin về kích thước của texture lưu các block tiles (texPath)
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	// tính toán số hàng, số cột cần thiết để đọc các ô tile từ file
	int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1;

	for (UINT i = 0; i < nums_rowToRead; i++)
	{
		for (UINT j = 0; j < nums_colToRead; j++)
		{
			int idTile = ID + id_sprite;
			sprites->Add(idTile, TILE_WIDTH * j, TILE_HEIGHT * i, TILE_WIDTH * (j + 1), TILE_HEIGHT * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
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

	while (!fs.eof())
	{
		getline(fs, line);

		// Lưu sprite tile vào vector tilemap
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

void CMaps::Add(LPCWSTR dataPath, LPCWSTR texPath, int ID, int width, int height)
{
	LPTILEMAP tilemap = new CMap(ID, texPath, dataPath, width, height);
	tilemaps[ID] = tilemap;
}

CMaps * CMaps::GetInstance()
{
	if (_instance == NULL) _instance = new CMaps();
	return _instance;
}
