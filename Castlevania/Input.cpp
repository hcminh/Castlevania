#include "Input.h"
#include "Scenes.h"
#include "Simon.h"

Input::Input()
{
	game = CGame::GetInstance();
}

bool Input::canUseKeyboard()
{
	if (CSimon::GetInstance()->GetState() == SIMON_STATE_DIE) return false;
	if (CSimon::GetInstance()->isJumping) return false;
	if (CSimon::GetInstance()->isAutoGoX) return false;
	if (CSimon::GetInstance()->isAttacking) return false;
	if (CSimon::GetInstance()->levelUpgrade) return false;
	return true;
}

bool Input::canPressKey()
{
	if (CSimon::GetInstance()->GetState() == SIMON_STATE_DIE) return false;
	if (CSimon::GetInstance()->isAutoGoX) return false;
	if (CSimon::GetInstance()->isAttacking) return false;
	if (CSimon::GetInstance()->levelUpgrade) return false;
	return true;
}

void Input::KeyState(BYTE *state)
{
	if (!canUseKeyboard()) return;

	if (game->IsKeyDown(DIK_DOWN))
		CSimon::GetInstance()->SetState(SIMON_STATE_SIT);
	else if (game->IsKeyDown(DIK_RIGHT))
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_U))
		CSimon::GetInstance()->SetState(SIMON_STATE_GO_UP_STAIR);
	else if (game->IsKeyDown(DIK_I))
		CSimon::GetInstance()->SetState(SIMON_STATE_IDLE);
	else
	{
		if (CSimon::GetInstance()->isSitting) CSimon::GetInstance()->SetState(SIMON_STATE_STANDUP);
		else if (CSimon::GetInstance()->isOnStair) CSimon::GetInstance()->SetState(SIMON_STATE_IDLE_UP_STAIR);
		else CSimon::GetInstance()->SetState(SIMON_STATE_IDLE);
	}
}

void Input::OnKeyDown(int KeyCode)
{
	if (!canPressKey()) return;
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		CSimon::GetInstance()->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_R: // reset
		CSimon::GetInstance()->SetState(SIMON_STATE_IDLE);
		CSimon::GetInstance()->SetPosition(50.0f, 0.0f);
		CSimon::GetInstance()->SetSpeed(0, 0);
		break;
	case DIK_A: // ATTACK
		CSimon::GetInstance()->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_Q:
		CSimon::GetInstance()->whip->levelUp();
		break;
	case DIK_X:
		DebugOut(L"[CORD] tọa độ X là: %f, Y là: %f \n", CSimon::GetInstance()->x, CSimon::GetInstance()->y);
		break;
	case DIK_0:
		DebugOut(L"[OBJECTS] Số lượng OBJ hiện tại là: %d \n", CScenes::GetInstance()->getObjectsSize());
		break;
	case DIK_1: //qua scene 1
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_1);
		CSimon::GetInstance()->SetPosition(10.0f, 300);
		CCamera::GetInstance()->SetCamPos(0.0f, 0.0f);
		break;
	case DIK_2: //qua scene 2
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_2);
		CSimon::GetInstance()->SetPosition(1200.0f, 300);
		break;
	case DIK_3: //qua scene 3
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_3);
		CSimon::GetInstance()->SetPosition(609.0f, 100);
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

