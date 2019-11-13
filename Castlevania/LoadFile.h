#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include "Textures.h"
using namespace std;

class CLoadFile
{
public:
	CLoadFile();
	wchar_t * ConvertToWideChar(char* p);
	void loadScenes(string path);
	void loadMaps(string path);
	void loadObject(string path);
	void loadSprites(string filepathtosprite, string filepathtotex, int idTex);
	void loadAnimations(string filepath, int idTex = 0);
	void LoadResources(string path);

};

