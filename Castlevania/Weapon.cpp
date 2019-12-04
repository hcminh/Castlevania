#include "Weapon.h"
#include "Simon.h"
#include "Scenes.h"
#include "Textures.h"
#include "debug.h"
#include "Zombie.h"


CWeapon::CWeapon() : CGameObject()
{
	type = ObjectType::WEAPON;
	isFlying = false;
	LoadResources();
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isFlying || isStopWatch) return;
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
	case WeaponType::HOLY_WATER_WEAPON:
		vy += HOLY_WATER_GRAVITY * dt;
		break;
	}

	CGameObject::Update(dt, coObjects);
	if (!isBurning)
	{
		x += dx;
		y += dy;
	}

	if (x > CCamera::GetInstance()->getCamPosX() + SCREEN_WIDTH || x < CCamera::GetInstance()->getCamPosX()
		|| y > CCamera::GetInstance()->getCamPosY() + SCREEN_HEIGHT)
	{
		isFlying = false;
	}

	if (state == WeaponType::FIRE_BALL)
	{
		if (CSimon::GetInstance()->isColisionWeapon(this))
		{
			isFlying = false;
			CSimon::GetInstance()->SetState(SIMON_STATE_ATTACKED);
		}
	}
	else
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			float top, left, bottom, right;
			coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
			if (isCollision(left, top, right, bottom))
			{
				if (coObjects->at(i)->type == ObjectType::ITEM)
				{
					coObjects->at(i)->SetState(CANDLE_STATE_DESTROYING);
					if (state == WeaponType::KNIFE_WEAPON || state == WeaponType::AXE_WEAPON) isFlying = false;
				}
				else if (coObjects->at(i)->type == ObjectType::ENEMY);
				{
					coObjects->at(i)->SetState(ZOMBIE_STATE_DEAD);
					if (state == WeaponType::KNIFE_WEAPON || state == WeaponType::AXE_WEAPON) isFlying = false;
				}
				if (state == WeaponType::HOLY_WATER_WEAPON)
				{
					SetState(WeaponType::FIRER);
				}
				break;

			}
		}
	}
}

void CWeapon::Render()
{
	if (!isFlying || isStopWatch) return;
	if (isBurning) ani = FIRE_ANI;
	else if (state == WeaponType::KNIFE_WEAPON) ani = KNIFE_ANI_RIGHT;
	else if (state == WeaponType::AXE_WEAPON)	ani = AXE_ANI_RIGHT;
	else if (state == WeaponType::HOLY_WATER_WEAPON)	ani = HOLY_WATER_ANI_RIGHT;
	else if (state == WeaponType::FIRE_BALL)	ani = FIRE_BALL_ANI_RIGHT;
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
	case WeaponType::HOLY_WATER_WEAPON:
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
	case WeaponType::FIRE_BALL:
		left = x;
		top = y;
		right = left + HOLY_WATER_BBOX; //sài lại cho lẹ
		bottom = top + HOLY_WATER_BBOX;
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
	case WeaponType::HOLY_WATER_WEAPON:
		vx = HOLY_WATER_SPEED_X * nx;
		vy = -HOLY_WATER_SPEED_Y;
		break;
	case WeaponType::STOP_WATCH_WEAPON:
		if (isStopWatch) return;
		isStopWatch = true;
		CScenes::GetInstance()->stopObject();
		break;
	case WeaponType::FIRER:
		vx = 0;
		vy = 0;
		accutime = 0;
		isBurning = true;
		break;
	case WeaponType::FIRE_BALL:
		vx = FIRE_BALL_SPEED * nx;
		vy = 0;
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
	AddAnimation(200);		// knife right
	AddAnimation(201);		// knife left

	AddAnimation(202);		// AXE right
	AddAnimation(203);		// AXE left

	AddAnimation(204);		// holy water RIGHT
	AddAnimation(205);		// holy water LEFT

	AddAnimation(206);		// cháy lúc holy water ném ra
	AddAnimation(206);		// add cái ani cháy 2 lần để cho nó đồng bộ vs mấy thằng kia, viết cái hàm render cho đẹp

	AddAnimation(207);		// cháy lúc holy water ném ra
	AddAnimation(208);		// add cái ani cháy 2 lần để cho nó đồng bộ vs mấy thằng kia, viết cái hàm render cho đẹp
}



