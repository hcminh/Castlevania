﻿#include "Input.h"
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
	if (simon->isAutoWalk2D) return false;
	if (simon->isAttacking) return false;
	if (simon->levelUpgrade) return false;
	if (simon->isFalling) return false;
	if (simon->isHurting) return false;
	return true;
}

bool Input::canPressKey()
{
	if (simon->GetState() == SIMON_STATE_DIE) return false;
	if (simon->isAutoWalk2D) return false;
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
		else if (simon->isAutoWalk2D) simon->SetState(SIMON_STATE_WALKING_RIGHT);
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
		CScenes::GetInstance()->usingHolyCross();
		break;
	case DIK_T: // reset
		CScenes::GetInstance()->usingStopWatch();
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
	case DIK_H:
		simon->nx = -simon->nx;
		break;
	case DIK_L:
		simon->x += 1.0f;
		break;
	case DIK_J:
		simon->x -= 1.0f;
		break;
	case DIK_I:
		simon->y -= 1.0f;
		break;
	case DIK_K:
		simon->y += 1.0f;
		break;
	case DIK_0:
		simon->SetState(SIMON_STATE_WALK);
		simon->vx = SIMON_AUTO_WALK_SPEED;
		simon->autoWalk2D(100, 0.0f, SIMON_STATE_IDLE, 1, false);
		break;
	case DIK_1: //qua scene 1
		CScenes::GetInstance()->changeScene(STAGEID::STAGE_1);
		break;
	case DIK_2: //qua scene 2
		CScenes::GetInstance()->changeScene(STAGEID::STAGE_2_1);
		break;
	case DIK_3: //qua scene 3
		CScenes::GetInstance()->changeScene(STAGEID::STAGE_2_2);
		break;
	case DIK_4: //qua scene 3
		CScenes::GetInstance()->changeScene(STAGEID::STAGE_2_3);
		break;
	case DIK_5: //qua scene 3
		CScenes::GetInstance()->changeScene(STAGEID::STAGE_3);
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
