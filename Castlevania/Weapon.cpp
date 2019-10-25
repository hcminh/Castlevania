#include "Weapon.h"
#include "Simon.h"

#include "Textures.h"
#include "debug.h"

//#include "Scenes.h"

CWeapon::CWeapon() : CGameObject()
{
	type = ObjectType::WEAPON;
	isFlying = false;
	LoadResources();
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isFlying) return;
	vx = KNIFE_FLYING_SPEED * nx;
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
			if (coObjects->at(i)->type == ObjectType::ITEM);
			{
				coObjects->at(i)->SetState(CANDLE_STATE_DESTROYING);
			}

			isFlying = false;
		}
	}
}

void CWeapon::Render()
{
	if (!isFlying) return;
	int ani = ((nx > 0) ? 0 : 1);
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

void CWeapon::setPosition(float x, float y)
{
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
	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(200);		// knife right
	AddAnimation(201);		// knife left
}



