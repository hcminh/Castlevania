#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include "define.h"

class CCamera
{
	static CCamera * __instance;
public:

	float camX = 0.0f;
	float camY = 0.0f;

	void SetCamPos(float x, float y) { camX = x; camY = y; };
	D3DXVECTOR3 getCamPos() { return D3DXVECTOR3(camX, camY, 0); };

	bool onCamera(float left, float right);

	float getCamPosX() { return camX; };
	float getCamPosY() { return camY; };

	float getBorderCamLeft() { return camX - 32; };
	float getBorderCamRight() { return camX + SCREEN_WIDTH + 32; };
	CCamera();
	static CCamera * GetInstance();
};

typedef CCamera * LPCAMERA;
