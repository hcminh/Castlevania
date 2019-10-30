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
	accutime += dt;
	if (isBurning && (accutime >= FIRER_TIME_DISPLAY))
	{
		isBurning = false;
		isFlying = false;
	}

	switch (state)
	{
	case WeaponType::AXE_WEAPON:
		vy += AXE_GRAVITY * dt;
		break;
	case WeaponType::HOLY_WATER:
		vy += HOLY_WATER_GRAVITY * dt;
		break;
	}

	CGameObject::Update(dt, coObjects);
	if (!isBurning)
	{
		x += dx;
		y += dy;
	}

	if (x > CGame::GetInstance()->getCamPosX() + SCREEN_WIDTH || x < CGame::GetInstance()->getCamPosX()
		|| y > CGame::GetInstance()->getCamPosY() + SCREEN_HEIGHT)
	{
		isFlying = false;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		float top, left, bottom, right;
		coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
		if (isCollision(left, top, right, bottom))
		{
			if (coObjects->at(i)->type == ObjectType::ITEM)
			{
				coObjects->at(i)->SetState(CANDLE_STATE_DESTROYING);
			}
			if (state == WeaponType::HOLY_WATER)
			{
				SetState(WeaponType::FIRER);
			}
			//else isFlying = false;
			break;
		}
	}
}

void CWeapon::Render()
{
	if (!isFlying) return;
	if (isBurning) ani = FIRE_ANI;
	else if (state == WeaponType::KNIFE_WEAPON) ani = KNIFE_ANI_RIGHT;
	else if (state == WeaponType::AXE_WEAPON)	ani = AXE_ANI_RIGHT;
	else if (state == WeaponType::HOLY_WATER)	ani = HOLY_WATER_ANI_RIGHT;
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
	case WeaponType::HOLY_WATER:
		left = x;
		top = y;
		right = left + HOLY_WATER_BBOX;
		bottom = top + HOLY_WATER_BBOX;
		break;
	case WeaponType::FIRER:
		left = x;
		top = y;
		right = left + FIRER_BBOX;
		bottom = top + FIRER_BBOX;
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
	case WeaponType::HOLY_WATER:
		vx = HOLY_WATER_SPEED_X * nx;
		vy = -HOLY_WATER_SPEED_Y;
		break;
	case WeaponType::FIRER:
		vx = 0;
		vy = 0;
		accutime = 0;
		isBurning = true;
		break;
	}
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

	AddAnimation(202);		// AXE right
	AddAnimation(203);		// AXE left

	AddAnimation(204);		// holy water RIGHT
	AddAnimation(205);		// holy water LEFT

	AddAnimation(206);		// cháy lúc holy water ném ra
	AddAnimation(206);		// add cái ani cháy 2 lần để cho nó đồng bộ vs mấy thằng kia, viết cái hàm render cho đẹp
}



