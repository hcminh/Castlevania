#include "Weapon.h"
#include "Simon.h"

#include "Textures.h"
#include "debug.h"

//#include "Scenes.h"

CWeapon::CWeapon() : CGameObject()
{
	type = ObjectType::WEAPON;
	isFlying = false;
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isFlying) return;
	if(direct == 0)
		vx = KNIFE_FLYING_SPEED;
	else if (direct == 1)
		vx = -KNIFE_FLYING_SPEED;
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	//DebugOut(L"[KNIFE] x là : %d\n", x);
	if (x > CGame::GetInstance()->getCamPosX() + SCREEN_WIDTH || x < CGame::GetInstance()->getCamPosX()) {
		isFlying = false;
	}

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

				isFlying = false;
			}
		}
}

void CWeapon::Render()
{
	if (!isFlying) return;
	if (direct < 0) return;
	int ani = direct; // level bang gia tri cua loai roi, direct = -1 -> ko danh, direct = 0 danh phai, direct = 1 danh trai
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
		left = x;
		top = y + 10;
		right = left + KNIFE_BBOX_WIDTH;
		bottom = top + KNIFE_BBOX_HEIGHT;
}

void CWeapon::setPosition(float x, float y, int direct)
{
	this->direct = direct;
	this->x = x;
	this->y = y;
}

bool CWeapon::isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void CWeapon::LoadResources()
{
	//CTextures * textures = CTextures::GetInstance();

	//textures->Add(ID_TEX_SUB_WEAPON, L"textures\\weapons-full.png", D3DCOLOR_XRGB(255, 0, 255));

	//CSprites * sprites = CSprites::GetInstance();
	//CAnimations * animations = CAnimations::GetInstance();

	//LPDIRECT3DTEXTURE9 texWeapon = textures->Get(ID_TEX_SUB_WEAPON);

	//// knife
	//sprites->Add(40020, 204, 32, 238, 50, texWeapon);		// right
	//sprites->Add(40021, 204, 0, 238, 18, texWeapon);		// left

	//LPANIMATION ani;

	////knife
	//ani = new CAnimation(100);	// right 
	//ani->Add(40020);
	//animations->Add(806, ani);

	//ani = new CAnimation(100);	// left 
	//ani->Add(40021);
	//animations->Add(807, ani);

	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(806);		// knife right
	AddAnimation(807);		// knife left
}



