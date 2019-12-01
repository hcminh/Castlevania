#include "Input.h"
#include "Scenes.h"

Input::Input()
{
	game = CGame::GetInstance();
}

void Input::addSimon()
{
	simon = CSimon::GetInstance();
}

bool Input::canUseKeyboard()
{
	if (simon->GetState() == SIMON_STATE_DIE) return false;
	if (simon->isJumping) return false;
	if (simon->isAutoGoX) return false;
	if (simon->isAttacking) return false;
	if (simon->levelUpgrade) return false;
	if (simon->isFalling) return false;
	if (simon->isHurting) return false;
	return true;
}

bool Input::canPressKey()
{
	if (simon->GetState() == SIMON_STATE_DIE) return false;
	if (simon->isAutoGoX) return false;
	if (simon->isAttacking) return false;
	if (simon->levelUpgrade) return false;
	if (simon->isHurting) return false;
	return true;
}

void Input::KeyState(BYTE *state)
{
	if (!canUseKeyboard()) return;

	if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->isColisionStair(CScenes::GetInstance()->stairs))
		{
			downStair();
			return;
		}
		simon->SetState(SIMON_STATE_SIT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_UP))
	{
		if (simon->isColisionStair(CScenes::GetInstance()->stairs))
		{
			upStair();
			return;
		}
		simon->SetState(SIMON_STATE_IDLE);
	}
	else
	{
		if (simon->isColisionStair(CScenes::GetInstance()->stairs))
		{
			if (standStair() == true)
				return;
		}
		else if (simon->isSitting)  simon->SetState(SIMON_STATE_STANDUP);
		simon->SetState(SIMON_STATE_IDLE);
	}
}

void Input::OnKeyDown(int KeyCode)
{
	if (!canPressKey()) return;
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_R: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	case DIK_A: // ATTACK
		simon->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_Q:
		simon->SetState(SIMON_STATE_LEVEL_UP);
		break;
	case DIK_W:
		simon->SetState(SIMON_STATE_INVISIBLE);
		break;
	case DIK_X:
		DebugOut(L"[CORD] tọa độ X là: %f, Y là: %f \n", simon->x, simon->y);
		break;
	case DIK_0:
		DebugOut(L"[OBJECTS] Số lượng OBJ hiện tại là: %d \n", CScenes::GetInstance()->getObjectsSize());
		break;
	case DIK_1: //qua scene 1
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_1);
		simon->SetPosition(10.0f, 300);
		CCamera::GetInstance()->SetCamPos(0.0f, 0.0f);
		break;
	case DIK_2: //qua scene 2
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_2);
		simon->SetPosition(1211.0f, 330);
		break;
	case DIK_P: //qua scene 2
		simon->SetPosition(1214.0f, 300);
		break;
	case DIK_3: //qua scene 3
		CScenes::GetInstance()->changeScene(SceneID::SCENEID_3);
		simon->SetPosition(609.0f, 100);
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void Input::downStair()
{
	int stairDirection = simon->stairDirection;

	if (simon->canMoveDownStair == false)
	{
		if (simon->isOnStair == true)
			simon->SetState(SIMON_STATE_IDLE);
		else
			simon->SetState(SIMON_STATE_SIT);

		return;
	}
	else
	{
		simon->nx = -simon->stairDirection;
		simon->SetState(SIMON_STATE_DOWN_STAIR);
	}

	return;
}

void Input::upStair()
{
	int stairDirection = simon->stairDirection;

	if (simon->canMoveUpStair == false)
	{
		if (simon->isOnStair == true)
		{
			int nx = simon->stairDirection;
			simon->nx = nx;
			simon->SetState(SIMON_STATE_UP_STAIR);
			simon->AutoWalk(14 * nx, SIMON_STATE_IDLE, nx);
		}
		return;
	}
	else
	{
		simon->nx = stairDirection;
		simon->SetState(SIMON_STATE_UP_STAIR);
	}

	return;
}

bool Input::standStair()
{
	if (simon->GetState() == SIMON_STATE_UP_STAIR || simon->GetState() == SIMON_STATE_DOWN_STAIR)
	{
		simon->vx = simon->vy = 0;
		return true;
	}

	return false;
}