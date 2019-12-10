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
	if (simon->isAutoWalk) return false;
	if (simon->isAttacking) return false;
	if (simon->levelUpgrade) return false;
	if (simon->isFalling) return false;
	if (simon->isHurting) return false;
	return true;
}

bool Input::canPressKey()
{
	if (simon->GetState() == SIMON_STATE_DIE) return false;
	if (simon->isAutoWalk) return false;
	if (simon->isAttacking) return false;
	if (simon->levelUpgrade) return false;
	if (simon->isHurting) return false;
	if (simon->isMoving) return false;
	return true;
}

void Input::KeyState(BYTE *state)
{
	if (!canUseKeyboard()) return;

	if (game->IsKeyDown(DIK_DOWN))
	{
		if(simon->downStair(CScenes::GetInstance()->stairs));
		else simon->SetState(SIMON_STATE_SIT);
	}
	else if (game->IsKeyDown(DIK_RIGHT) && !simon->isOnStair)
	{
		simon->nx = 1;
		simon->SetState(SIMON_STATE_WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT) && !simon->isOnStair)
	{
		simon->nx = -1;
		simon->SetState(SIMON_STATE_WALK);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		simon->upStair(CScenes::GetInstance()->stairs);
	}
	else
	{
		if(simon->isOnStair) simon->SetState(SIMON_STATE_IDLE_STAIR);
		else if (simon->isSitting)  simon->SetState(SIMON_STATE_STANDUP);
		else if (simon->isAutoWalk) simon->SetState(SIMON_STATE_WALKING_RIGHT);
		else simon->SetState(SIMON_STATE_IDLE);
	}
}

void Input::OnKeyDown(int KeyCode)
{
	if (!canPressKey()) return;
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->isOnStair) return;
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
		simon->SetState(SIMON_STATE_AUTO_WALK);
		break;
	case DIK_1: //qua scene 1
		CScenes::GetInstance()->changeScene(SCENEID::SCENEID_1);
		break;
	case DIK_2: //qua scene 2
		CScenes::GetInstance()->changeScene(SCENEID::SCENEID_2);
		break;
	case DIK_P: //qua scene 2
		simon->SetPosition(1214.0f, 300);
		break;
	case DIK_3: //qua scene 3
		CScenes::GetInstance()->changeScene(SCENEID::SCENEID_3);
		simon->SetPosition(10.0f, 100);
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
