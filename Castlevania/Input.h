#pragma once

#include "Game.h"

class Input : public CKeyEventHandler
{
	CGame * game;
public:
	Input();
	bool canUseKeyboard();
	bool canPressKey();
	virtual void KeyState(BYTE *state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

