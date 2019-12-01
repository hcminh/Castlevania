#pragma once

#include "Game.h"
#include "Simon.h"

class Input : public CKeyEventHandler
{
	CGame * game;
	CSimon * simon;
public:
	Input();
	void addSimon();
	bool canUseKeyboard();
	bool canPressKey();
	virtual void KeyState(BYTE *state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void downStair();
	void upStair();
	bool standStair();
};

