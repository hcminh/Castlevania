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
//#include "Candle.h"
#include "Item.h"
#include "Ground.h"
#include "define.h"
#include "tilemap.h"
#include "Door.h"

#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

CGame *game;
void loadObject(string filepath);
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
	case DIK_1: //qua scene 2
		CScenes::GetInstance()->setCurrentScene(SCENE_1);
		loadObject("textures\\map\\scene1-objects.txt");
		break;
	case DIK_2: //qua scene 2
		CScenes::GetInstance()->setCurrentScene(SCENE_2);
		loadObject("textures\\map\\scene2-objects.txt");
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (CSimon::GetInstance()->GetState() == SIMON_STATE_DIE) return;
	if (CSimon::GetInstance()->GetState() == SIMON_STATE_JUMP && CSimon::GetInstance()->isJumping) return;
	if (CSimon::GetInstance()->isAutoGoX) return;

	if (game->IsKeyDown(DIK_DOWN) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_SIT);
	else if (game->IsKeyDown(DIK_RIGHT) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT) && !CSimon::GetInstance()->isAttacking)
		CSimon::GetInstance()->SetState(SIMON_STATE_WALKING_LEFT);
	else
	{
		if (CSimon::GetInstance()->isSitting) CSimon::GetInstance()->SetState(SIMON_STATE_STANDUP);
		CSimon::GetInstance()->SetState(SIMON_STATE_IDLE);
	}
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

wchar_t* ConvertToWideChar(char* p) // hàm này covert string sang wchar_t*, coppy trên mạng
{
	wchar_t *r;
	r = new wchar_t[strlen(p) + 1];

	char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);

	return r;
}

void loadSprites(string filepathtosprite, string filepathtotex, int idTex)
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	textures->Add(idTex, ConvertToWideChar((char*)filepathtotex.c_str()), D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 tex = textures->Get(idTex);
	fstream fs;
	fs.open(filepathtosprite, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file sprite lỗi");
		fs.close();
	}
	while (!fs.eof())
	{
		int sprite, left, top, right, bottom;
		fs >> sprite >> left >> top >> right >> bottom;
		sprites->Add(sprite, left, top, right, bottom, tex);
	}
	fs.close();
}

void loadAnimations(string filepath, int idTex = 0) {
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;
	fstream fs;
	fs.open(filepath, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file animation lỗi");
		fs.close();
	}
	while (!fs.eof())
	{
		int id;
		int sprite1 = -1, sprite2 = -1, sprite3 = -1, sprite4 = -1;
		fs >> id >> sprite1 >> sprite2 >> sprite3 >> sprite4;
		ani = new CAnimation(100);
		if (sprite1 > -1)ani->Add(sprite1);
		if (sprite2 > -1)ani->Add(sprite2);
		if (sprite3 > -1)ani->Add(sprite3);
		if (sprite4 > -1)ani->Add(sprite4);
		animations->Add(id, ani);
	}
	fs.close();
}

void loadObject(string filepath) {
	CScenes::GetInstance()->clearAllObject();
	//nhét con simon vào đầu mảng cho dễ xử lý 
	CScenes::GetInstance()->insertObject(CSimon::GetInstance());
	fstream fs;
	fs.open(filepath, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file obecject lỗi");
		fs.close();
	}
	int id;
	int item;
	float x, y;
	while (!fs.eof())
	{
		fs >> id >> x >> y >> item;
		switch (id)
		{
		case ID_ITEM:
		{
			CItem *cItem = new CItem(ItemType(item), ItemState::STATE_BIG_CANDLE);
			cItem->SetPosition(x, y);
			CScenes::GetInstance()->pushObject(cItem);
			break;
		}
		case ID_GROUND:
		{
			CGround *ground = new CGround(x, y);
			CScenes::GetInstance()->pushObject(ground);
			break;
		}
		case ID_DOOR:
		{
			CDoor *door = new CDoor(x, y);
			CScenes::GetInstance()->pushObject(door);
			break;
		}
		}
	}
	fs.close();
}

void LoadResources()
{
	fstream fs;
	fs.open("textures\\index.txt", ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Load file sprite lỗi");
		fs.close();
	}
	while (!fs.eof())
	{
		int id;
		string tex, sprite, animation;
		fs >> id >> tex >> sprite >> animation;
		if (id == ID_TEX_BBOX) {
			CTextures::GetInstance()->Add(id, ConvertToWideChar((char*)tex.c_str()), D3DCOLOR_XRGB(255, 0, 255));
		}
		else if (id == SCENE_1)
		{
			CMaps::GetInstance()->Add(ConvertToWideChar((char*)sprite.c_str()), ConvertToWideChar((char*)tex.c_str()), id, MAP_1_WITDH, MAP_1_HEIGHT);
		}
		else if (id == SCENE_2)
		{
			CMaps::GetInstance()->Add(ConvertToWideChar((char*)sprite.c_str()), ConvertToWideChar((char*)tex.c_str()), id, MAP_2_WITDH, MAP_2_HEIGHT);
		}
		else if (id == OBJECT_SCENE_1)
		{
			loadObject(sprite);
		}
		else
		{
			loadSprites(sprite, tex, id);
			loadAnimations(animation);
		}
	}
	fs.close();
}

void Update(DWORD dt)
{/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
	CScenes::GetInstance()->Update(dt);
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

		CScenes::GetInstance()->Render();

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

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	Run();

	return 0;
}