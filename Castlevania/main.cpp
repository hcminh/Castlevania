#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "define.h"
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Scenes.h"

#include "Simon.h"
#include "Brick.h"
#include "Goomba.h"
#include "Whip.h"
#include "Candle.h"
#include "Item.h"
#include "Ground.h"

#include "tilemap.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON	0
#define ID_TEX_WHIP		5
#define ID_TEX_ENEMY	20
#define ID_TEX_MISC		30
#define ID_TEX_CANDLE	35
#define ID_TEX_ITEM		40
#define ID_TEX_GROUND	45


CGame *game;
CScenes *scenes;

CCandle *candle;
//CItem *item;

CMaps * cmaps = CMaps::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
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
	case DIK_W:
		CSimon::GetInstance()->whip->levelDown();
		break;
	case DIK_Z: //sub attack
		if (CSimon::GetInstance()->subWeapon != NULL && CSimon::GetInstance()->subWeapon->isFlying) {
			CSimon::GetInstance()->SetState(SIMON_STATE_ATTACK);
			return;
		}

		CSimon::GetInstance()->SetState(SIMON_STATE_ATTACK_WITH_SUB);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_DOWN:
		CSimon::GetInstance()->SetState(SIMON_STATE_STANDUP);
		break;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Simon die 
	if (CSimon::GetInstance()->GetState() == SIMON_STATE_DIE) return;
	//disable when jump
	//if (simon->GetState() == SIMON_STATE_JUMP) return;

	if (game->IsKeyDown(DIK_RIGHT) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_SIT);
	else
		CSimon::GetInstance()->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	cmaps->Add(SCENE_1, L"textures\\scene1.png", L"textures\\scene1_map.txt");
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 408, 225, 424, 241, texMisc);


	LPANIMATION ani;

	////////////////////////// Item ///////////////////////////
	textures->Add(ID_TEX_ITEM, L"textures\\Items.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texItem = textures->Get(ID_TEX_ITEM);
	sprites->Add(60000, 16, 0, 40, 20, texItem);	// big heart
	sprites->Add(60001, 0, 0, 16, 16, texItem);	// small heart
	sprites->Add(60002, 130, 0, 162, 32, texItem);	// whip
	sprites->Add(60003, 162, 0, 194, 18, texItem);	// knife

	ani = new CAnimation(100);	// big heart
	ani->Add(60000);
	animations->Add(900, ani);

	ani = new CAnimation(100);	// small heart
	ani->Add(60001);
	animations->Add(901, ani);

	ani = new CAnimation(100);	// whip
	ani->Add(60002);
	animations->Add(902, ani);

	ani = new CAnimation(100);	// knife
	ani->Add(60003);
	animations->Add(903, ani);

	////////////////////////// Item ///////////////////////////

	//////////////////////////  Weapon  ///////////////////////////

	textures->Add(ID_TEX_SUB_WEAPON, L"textures\\weapons-full.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texWeapon = textures->Get(ID_TEX_SUB_WEAPON);
	// knife
	sprites->Add(40020, 204, 32, 238, 50, texWeapon);		// right
	sprites->Add(40021, 204, 0, 238, 18, texWeapon);		// left

	//knife
	ani = new CAnimation(100);	// right 
	ani->Add(40020);
	animations->Add(806, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40021);
	animations->Add(807, ani);
	//////////////////////////  Weapon  ///////////////////////////

	//simon
	scenes->pushObject(CSimon::GetInstance());

	//////////////////////////  GROUND  ///////////////////////////
	textures->Add(ID_TEX_GROUND, L"textures\\scene1.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texGround = textures->Get(ID_TEX_GROUND);
	sprites->Add(20001, 320, 96, 320 + 32, 96 + 32, texGround);		// GROUND
	ani = new CAnimation(100);		// brick
	ani->Add(20001);
	animations->Add(601, ani);
	for (int i = 0; i < 48; i++)
	{
		CGround *ground = new CGround();
		ground->SetPosition(i * 32.0f, SCREEN_HEIGHT - 115);
		scenes->pushObject(ground);
	}
	//////////////////////////  GROUND  ///////////////////////////


	//candle 1
	candle = new CCandle(160.0, 304.0, ItemType::BIG_HEART);
	scenes->pushObject(candle);
	//candle 2
	candle = new CCandle(448.0, 304.0, ItemType::WHIP);
	scenes->pushObject(candle);
	//candle 3
	candle = new CCandle(672.0, 304.0, ItemType::WHIP);
	scenes->pushObject(candle);
	//candle 4
	candle = new CCandle(960.0, 304.0, ItemType::BIG_HEART);
	scenes->pushObject(candle);
	//candle 5
	candle = new CCandle(1216.0, 304.0, ItemType::KNIFE);
	scenes->pushObject(candle);

}

void Update(DWORD dt)
{/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
	scenes->Update(dt);
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		scenes->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	scenes = CScenes::GetInstance();
	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	Run();

	return 0;
}