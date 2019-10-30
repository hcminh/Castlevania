#include "Weapon.h"
#include "Simon.h"

#include "Textures.h"
#include "debug.h"


CWeapon::CWeapon() : CGameObject()
{
	type = ObjectType::WEAPON;
	isFlying = false;
	LoadResources();
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isFlying) return;

	switch (state)
	{
	case WeaponType::AXE_WEAPON:
		vy += AXE_GRAVITY * dt;
		break;
	}

	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

	if (x > CGame::GetInstance()->getCamPosX() + SCREEN_WIDTH || x < CGame::GetInstance()->getCamPosX()
		|| y > SCREEN_HEIGHT)
	{
		isFlying = false;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		float top, left, bottom, right;
		coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
		if (isCollision(left, top, right, bottom))
		{
			if (coObjects->at(i)->type == ObjectType::ITEM);
			{
				DebugOut(L"[KNIFE] CHẠM VỚI CÁI GÌ TẠI: %f \n", x);
				coObjects->at(i)->SetState(CANDLE_STATE_DESTROYING);
				isFlying = false;
			}
			break;
		}
	}
}

void CWeapon::Render()
{
	if (!isFlying) return;
	if (state == WeaponType::KNIFE_WEAPON) ani = KNIFE_ANI_RIGHT;
	else if (state == WeaponType::AXE_WEAPON) ani = AXE_ANI_RIGHT;
	ani += ((nx > 0) ? 0 : 1);
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	switch (state)
	{
	case WeaponType::AXE_WEAPON:
		left = x;
		top = y;
		right = left + AXE_BBOX;
		bottom = top + AXE_BBOX;
		break;
	case WeaponType::KNIFE_WEAPON:
		left = x;
		top = y;
		right = left + KNIFE_BBOX_WIDTH;
		bottom = top + KNIFE_BBOX_HEIGHT;
		break;
	}
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WeaponType::KNIFE_WEAPON:
		vx = KNIFE_FLYING_SPEED * nx;
		vy = 0;
		break;
	case WeaponType::AXE_WEAPON:
		vx = AXE_SPEED_X * nx;
		vy = -AXE_SPEED_Y;
		break;
	}
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


	AddAnimation(202);		// knife right
	AddAnimation(203);		// knife left
}



