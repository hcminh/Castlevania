#include "LoadFile.h"
#include "Scenes.h"
#include "debug.h"



CLoadFile::CLoadFile()
{
}

wchar_t * CLoadFile::ConvertToWideChar(char * p)
{
	wchar_t *r;
	r = new wchar_t[strlen(p) + 1];

	char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);

	return r;
}

void CLoadFile::loadScenes(string path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file scene lỗi \n");
		fs.close();
	}
	while (!fs.eof())
	{
		int sceneID, mapID;
		string linkObjs;
		fs >> sceneID >> mapID >> linkObjs;
		CScenes::GetInstance()->Add(SceneID(sceneID), mapID, linkObjs);
	}
	fs.close();
}

void CLoadFile::loadMaps(string path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load map lỗi \n");
		fs.close();
	}
	while (!fs.eof())
	{
		int mapID;
		string mapTex, mapSprite, mapMatrix;
		fs >> mapID >> mapTex >> mapSprite >> mapMatrix;
		loadSprites(mapSprite, mapTex, mapID);
		CMaps::GetInstance()->Add(ConvertToWideChar((char*)mapMatrix.c_str()), mapID);
	}
	fs.close();
}

void CLoadFile::loadObject(string path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load sprite, animation của object lỗi \n");
		fs.close();
	}
	while (!fs.eof())
	{
		int objID;
		string tex, sprite, animation;
		fs >> objID >> tex >> sprite >> animation;
		loadSprites(sprite, tex, objID);
		loadAnimations(animation);
	}
	fs.close();
}

void CLoadFile::loadSprites(string filepathtosprite, string filepathtotex, int idTex)
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	textures->Add(idTex, ConvertToWideChar((char*)filepathtotex.c_str()), D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 tex = textures->Get(idTex);
	fstream fs;
	fs.open(filepathtosprite, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file sprite lỗi Ở ID: %d \n", idTex);
		fs.close();
	}
	while (!fs.eof())
	{
		int sprite, left, top, right, bottom;
		fs >> sprite >> left >> top >> right >> bottom;
		sprites->Add(sprite, left, top, right, bottom, tex);
	}
	fs.close();
}

void CLoadFile::loadAnimations(string filepath, int idTex)
{
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;
	fstream fs;
	fs.open(filepath, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file animation lỗi");
		fs.close();
	}
	while (!fs.eof())
	{
		int id, timeAnimation;
		int sprite1 = -1, sprite2 = -1, sprite3 = -1, sprite4 = -1;
		fs >> id >> sprite1 >> sprite2 >> sprite3 >> sprite4 >> timeAnimation;
		ani = new CAnimation(timeAnimation);
		if (sprite1 > -1)ani->Add(sprite1);
		if (sprite2 > -1)ani->Add(sprite2);
		if (sprite3 > -1)ani->Add(sprite3);
		if (sprite4 > -1)ani->Add(sprite4);
		animations->Add(id, ani);
	}
	fs.close();
}

void CLoadFile::LoadResources(string path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load source lỗi");
		fs.close();
	}
	while (!fs.eof())
	{
		int id;
		string path;
		fs >> id >> path;
		switch (id)
		{
		case ID_BBOX:
			CTextures::GetInstance()->Add(id, ConvertToWideChar((char*)path.c_str()), D3DCOLOR_XRGB(255, 0, 255));
			break;
		case ID_MAP:
			loadMaps(path);
			break;
		case ID_SCENE:
			loadScenes(path);
			break;
		case ID_OBJECT:
			loadObject(path);
			break;
		default:
			break;
		}
	}
	fs.close();
}

