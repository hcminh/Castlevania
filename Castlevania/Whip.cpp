#include "Whip.h"
#include "Textures.h"

CWhip::CWhip() : CGameObject()
{
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	
}

void CWhip::Render()
{
	if (direct < 0) return;
	int ani;
	ani = level + direct; // level bang gia tri cua loai roi, direct = -1 -> ko danh, direct = 0 danh phai, direct = 1 danh trai
	animations[ani]->Render(x, y, 255);
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y + 15; //từ đầu xuống tay của con simon là 15
	right = left + WHIP_BBOX_WIDTH;
	bottom = top + WHIP_BBOX_HEIGHT;
}

void CWhip::setPosition(float x, float y, int direct)
{
	this->direct = direct;
	this->x = x ; 
	this->y = y;
}

void CWhip::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_WHIP, L"textures\\whip-full.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texWhip = textures->Get(ID_TEX_WHIP);

	// normal whip NORMAL_WHIP
	sprites->Add(40001, WHIP_WIDTH * 6, 0, WHIP_WIDTH * 7, WHIP_HEIGHT, texWhip);		// right
	sprites->Add(40002, WHIP_WIDTH * 5, 0, WHIP_WIDTH * 6, WHIP_HEIGHT, texWhip);
	sprites->Add(40003, WHIP_WIDTH * 4, 0, WHIP_WIDTH * 5, WHIP_HEIGHT, texWhip);

	sprites->Add(40007, WHIP_WIDTH * 3, 0, WHIP_WIDTH * 3 + WHIP_WIDTH, WHIP_HEIGHT, texWhip);		// blank frame

	sprites->Add(40004, WHIP_WIDTH * 0, 0, WHIP_WIDTH * 0 + WHIP_WIDTH, WHIP_HEIGHT, texWhip);		// left
	sprites->Add(40005, WHIP_WIDTH * 1, 0, WHIP_WIDTH * 1 + WHIP_WIDTH, WHIP_HEIGHT, texWhip);
	sprites->Add(40006, WHIP_WIDTH * 2, 0, WHIP_WIDTH * 2 + WHIP_WIDTH, WHIP_HEIGHT, texWhip);


	// short whip SHORT_CHAIN	
	sprites->Add(40008, WHIP_WIDTH * 6, WHIP_HEIGHT * 1, WHIP_WIDTH * 7, WHIP_HEIGHT * 2, texWhip);		// right
	sprites->Add(40009, WHIP_WIDTH * 5, WHIP_HEIGHT * 1, WHIP_WIDTH * 6, WHIP_HEIGHT * 2, texWhip);
	sprites->Add(40010, WHIP_WIDTH * 4, WHIP_HEIGHT * 1, WHIP_WIDTH * 5, WHIP_HEIGHT * 2, texWhip);

	sprites->Add(40011, WHIP_WIDTH * 0, WHIP_HEIGHT * 1, WHIP_WIDTH * 1, WHIP_HEIGHT * 2, texWhip);		// left
	sprites->Add(40012, WHIP_WIDTH * 1, WHIP_HEIGHT * 1, WHIP_WIDTH * 2, WHIP_HEIGHT * 2, texWhip);
	sprites->Add(40013, WHIP_WIDTH * 2, WHIP_HEIGHT * 1, WHIP_WIDTH * 3, WHIP_HEIGHT * 2, texWhip);

	// long whip LONG_CHAIN	
	sprites->Add(40014, WHIP_WIDTH * 6, WHIP_HEIGHT * 2, WHIP_WIDTH * 7, WHIP_HEIGHT * 3, texWhip);		// right
	sprites->Add(40015, WHIP_WIDTH * 5, WHIP_HEIGHT * 2, WHIP_WIDTH * 6, WHIP_HEIGHT * 3, texWhip);
	sprites->Add(40016, WHIP_WIDTH * 4, WHIP_HEIGHT * 2, WHIP_WIDTH * 5, WHIP_HEIGHT * 3, texWhip);

	sprites->Add(40017, WHIP_WIDTH * 0, WHIP_HEIGHT * 2, WHIP_WIDTH * 1, WHIP_HEIGHT * 3, texWhip);		// left
	sprites->Add(40018, WHIP_WIDTH * 1, WHIP_HEIGHT * 2, WHIP_WIDTH * 2, WHIP_HEIGHT * 3, texWhip);
	sprites->Add(40019, WHIP_WIDTH * 2, WHIP_HEIGHT * 2, WHIP_WIDTH * 3, WHIP_HEIGHT * 3, texWhip);

	LPANIMATION ani;

	//normal whip
	ani = new CAnimation(100);	// right 
	ani->Add(40001);
	ani->Add(40002);
	ani->Add(40003);
	ani->Add(40007);
	animations->Add(700, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40004);
	ani->Add(40005);
	ani->Add(40006);
	ani->Add(40007);
	animations->Add(701, ani);

	// short whip SHORT_CHAIN
	ani = new CAnimation(100);	// right 
	ani->Add(40008);
	ani->Add(40009);
	ani->Add(40010);
	ani->Add(40007);
	animations->Add(702, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40011);
	ani->Add(40012);
	ani->Add(40013);
	ani->Add(40007);
	animations->Add(703, ani);

	// long whip LONG_CHAIN	
	ani = new CAnimation(100);	// right 
	ani->Add(40014);
	ani->Add(40015);
	ani->Add(40016);
	ani->Add(40007);
	animations->Add(704, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40017);
	ani->Add(40018);
	ani->Add(40019);
	ani->Add(40007);
	animations->Add(705, ani);


	AddAnimation(700);		// normal whip right
	AddAnimation(701);		// normal whip left

	AddAnimation(702);		// short whip right
	AddAnimation(703);		// short whip left

	AddAnimation(704);		// long whip right
	AddAnimation(705);		// long whip left
}



