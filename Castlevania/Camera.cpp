#include "Camera.h"
#include "Simon.h"
#include "Scenes.h"

CCamera * CCamera::__instance = NULL;

bool CCamera::onCamera(float left, float right)
{
	if (left < camX - 60 && (right < camX - 60)) return false;
	if (left > camX + SCREEN_WIDTH + 60) return false;
	return true;
}

bool CCamera::isMovingCamera()
{
	if (camX < newX)
	{
		camX++;
		return true;
	}
	autoMoving = false;
	return false;
}

void CCamera::movingCamera(float newX)
{
	this->newX = newX;
	autoMoving = true;
}

void CCamera::update(float stateWidth, float startPointOfState)
{
	if (autoMoving && isMovingCamera())
		return;
	CScenes::GetInstance()->stopMovingObject = false;
	float xSimon = CSimon::GetInstance()->x + 30; //nửa người simon

	if (xSimon - startPointOfState > SCREEN_WIDTH / 2 &&
		xSimon + SCREEN_WIDTH / 2 < stateWidth)
	{
		SetCamPos(xSimon - SCREEN_WIDTH / 2, 0);
	}
	else if (xSimon - startPointOfState < SCREEN_WIDTH / 2)
		SetCamPos(startPointOfState, 0);
	else if (xSimon > SCREEN_WIDTH / 2) SetCamPos(stateWidth - SCREEN_WIDTH - 4, 0);
}

CCamera::CCamera()
{
}

CCamera *CCamera::GetInstance()
{
	if (__instance == NULL) __instance = new CCamera();
	return __instance;
}
