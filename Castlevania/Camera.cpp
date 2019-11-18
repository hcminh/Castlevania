#include "Camera.h"



CCamera * CCamera::__instance = NULL;

bool CCamera::onCamera(float left, float right)
{
	if (left < camX - 32 && (right < camX - 32)) return false;
	if (left > camX + SCREEN_WIDTH + 32) return false;
	return true;
}

CCamera::CCamera()
{
}

CCamera *CCamera::GetInstance()
{
	if (__instance == NULL) __instance = new CCamera();
	return __instance;
}
