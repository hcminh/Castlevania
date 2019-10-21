#include "Whip.h"
#include "Textures.h"

//#include "Scenes.h"

CWhip::CWhip() : CGameObject()
{
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (animations[ani]->getCurrentFrame() == 2)
	{	
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->type == ObjectType::BRICK) continue;
			float top, left, bottom, right;
			coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
			if (isCollision(left, top, right, bottom))
			{
				if (coObjects->at(i)->type == ObjectType::CANDLE);
				{
					coObjects->at(i)->SetState(CANDLE_DESTROYED);
					//CScenes::GetInstance()->putItem(coObjects->at(i)->item);
				}
			}


		}
	}
}

void CWhip::Render()
{
	if (direct < 0) return;
	ani = level + direct; // level bang gia tri cua loai roi, direct = -1 -> ko danh, direct = 0 danh phai, direct = 1 danh trai
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if(direct == 0)
	{
		left = x + RANGE_OF_WHIP_SIMON_X + 32; //32 là khoảng cách từ x simon (biên bên trái) tới vị trí tay của simon (biên bên phải)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH); //cái này kiểm tra nếu level của whip >= 3 (Long chain) thì witdh dài hơn, chỉ kiểm tra 2 trường hợp vì short chain = normal whip
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else if(direct == 1)
	{
		left = x + RANGE_OF_WHIP_SIMON_X - 7 - ((level >= 3) ? RANGE_OF_LONG_NORMAL : 0); //7 là sai số khi dịch x lại gần biên bên trái của simon (vì nó chìa cái tay ra nữa)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH);
		bottom = top + WHIP_BBOX_HEIGHT;
	}
}

void CWhip::setPosition(float x, float y, int direct)
{
	this->direct = direct;
	this->x = x ; 
	this->y = y;
}

bool CWhip::isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
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
	animations->Add(800, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40004);
	ani->Add(40005);
	ani->Add(40006);
	ani->Add(40007);
	animations->Add(801, ani);

	// short whip SHORT_CHAIN
	ani = new CAnimation(100);	// right 
	ani->Add(40008);
	ani->Add(40009);
	ani->Add(40010);
	ani->Add(40007);
	animations->Add(802, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40011);
	ani->Add(40012);
	ani->Add(40013);
	ani->Add(40007);
	animations->Add(803, ani);

	// long whip LONG_CHAIN	
	ani = new CAnimation(100);	// right 
	ani->Add(40014);
	ani->Add(40015);
	ani->Add(40016);
	ani->Add(40007);
	animations->Add(804, ani);

	ani = new CAnimation(100);	// left 
	ani->Add(40017);
	ani->Add(40018);
	ani->Add(40019);
	ani->Add(40007);
	animations->Add(805, ani);


	AddAnimation(800);		// normal whip right
	AddAnimation(801);		// normal whip left

	AddAnimation(802);		// short whip right
	AddAnimation(803);		// short whip left

	AddAnimation(804);		// long whip right
	AddAnimation(805);		// long whip left
}



