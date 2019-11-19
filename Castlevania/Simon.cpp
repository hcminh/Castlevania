﻿#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "Scenes.h"
#include <math.h>

CSimon * CSimon::__instance = NULL;
CSimon *CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	__instance->isEnable = true;
	return __instance;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	
	//being attacked
	//if (isAttacked && accuTime <= SIMON_HURT_TIME) {
	//	x -= 3.0f*nx;
	//	y -= 2.0f;
	//	return;
	//}
	//isAttacked = false;

	// Calculate dx, dy 
	CGameObject::Update(dt);
	vy += SIMON_GRAVITY * dt;
	vector<LPGAMEOBJECT> listObject; // lọc danh sách có khả năng va chạm
	listObject.clear();
	for (UINT i = 0; i < coObjects->size(); i++) {
		if(coObjects->at(i)->type != ObjectType::ITEM)
			listObject.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(&listObject, coEvents);

	//upgrade whip level
	if (levelUpgrade && (GetTickCount() - levelUpStart > SIMON_UP_LEVEL_TIME))
	{
		levelUpStart = 0;
		levelUpgrade = false;
	}

	//hurting
	if (isHurting && (GetTickCount() - hurtingStart > SIMON_HURTING_TIME))
	{
		hurtingStart = 0;
		isHurting = false;
	}	

	 //reset untouchable timer if untouchable time has passed
	if (untouchable && (GetTickCount() - untouchableStart > SIMON_UNTOUCHABLE_TIME))
	{
		untouchableStart = 0;
		untouchable = false;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0 && !untouchable)
		{
			//vx = 0;
			//Collision logic with DOOR
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->obj->type == ObjectType::DOOR) // nếu e->obj là DOOR
				{
					DebugOut(L"Qua màn mới nè!!!");
					CScenes::GetInstance()->changeScene();
				}
				else if (e->obj->type == ObjectType::GROUND) // nếu e->obj là DOOR
				{
					x -= min_tx * dx + nx * 0.4f;
				}
				else if (e->obj->type == ObjectType::ENEMY) // nếu e->obj là DOOR
				{
					SetState(SIMON_STATE_ATTACKED);
				}
			}
		}
		if (ny < 0) {
			vy = 0;
			if (isJumping) 
			{
				SetState(SIMON_STATE_STANDUP);
				isJumping = false;
			}
		}
		else if (ny > 0)
			y += dy;
	}

	//update weapon
	whip->Update(dt, coObjects);
	if (subWeapon != NULL) subWeapon->Update(dt, coObjects);
	if (isAttacking && animations[ani]->getCurrentFrame() >= MAX_ATTACK_FRAME) //nếu là frame đánh cuói cùng thì tắt isAttacking
	{
		isAttacking = false;
		isUseSubWeapon = false;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	ani = SIMON_ANI_IDLE_RIGHT;
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);

	if (state == SIMON_STATE_DIE) ani = SIMON_ANI_DIE;
	else if (levelUpgrade) color = D3DCOLOR_ARGB(255, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
	else if (isHurting) ani = SIMON_ANI_HURT_RIGHT;
	else if (isAttacking && isSitting)  ani = SIMON_ANI_SIT_ATTACK_RIGHT;
	else if (isAttacking)				ani = SIMON_ANI_ATTACK_RIGHT;
	else if (isJumping)					ani = SIMON_ANI_JUMP_RIGHT;
	else if (isSitting)					ani = SIMON_ANI_SIT_RIGHT;
	else if (vx != 0) ani = SIMON_ANI_WALKING_RIGHT;

	if(untouchable) color = D3DCOLOR_ARGB(rand() % 200 + 50, 255, 255, 255);
	ani += (nx > 0) ? 0 : 1; //vì hành động phải chỉ cách hành động trái 1 frame nên sét ani bằng phải rồi kiểm tra nx là dc
	animations[ani]->Render(x, y, color);

	//xử lý render sau khi đã bấm nút
	if (typeSubWeapon != WeaponType::NONE && subWeapon->isFlying) subWeapon->Render();

	//xử lý render vũ khí khi vừa bấm nút
	if (isAttacking && !isUseSubWeapon)
	{
		whip->Render();
	}
	//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	if (isAttacking || isAutoGoX) return;
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		if (isSitting) SetState(SIMON_STATE_STANDUP);
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		if (isSitting) SetState(SIMON_STATE_STANDUP);
		nx = -1;
		break;
	case SIMON_STATE_SIT:
		if (isSitting || isAttacking)return;
		isSitting = true;
		y += 18;
		vx = 0;
		break;
	case SIMON_STATE_STANDUP:
		isSitting = false;
		isJumping = false;
		y -= 18;
		vx = 0;
		break;
	case SIMON_STATE_JUMP:
		if (isJumping)return;
		isJumping = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_ATTACK:
		attack();
		break;
	case SIMON_STATE_IDLE:
		isOnStair = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_LEVEL_UP:
		if (levelUpgrade) return;
		whip->levelUp();
		startLevelUp();
		break;
	case SIMON_STATE_AUTO_GO:
		if (isAutoGoX) return;
		autoGotoX(1385.0);
		break;
	case SIMON_STATE_ATTACKED:
		vx = -0.5*nx;
		vy = -0.3f;
		startHurting();
		startUntouchable();
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
		if (isSitting || (isJumping && !isAttacking))
		{
			left = x + PADDING;
			top = y;
			right = x + SIMON_BBOX_WIDTH - PADDING;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		} 
		else
		{
			left = x + PADDING;
			top = y;
			right = x + SIMON_BBOX_WIDTH - PADDING;
			bottom = top + SIMON_BBOX_HEIGHT;
		}
}

bool CSimon::isColisionItem(CItem *item)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);  // lấy BBOX của simon

	item->GetBoundingBox(l1, t1, r1, b1);
	if (CGameObject::AABB(l, t, r, b, l1, t1, r1, b1))
	{
		return true; // check with AABB
	}
	LPCOLLISIONEVENT e = SweptAABBEx(item); // kt va chạm giữa 2 object bằng sweptAABB
	bool res = e->t > 0 && e->t <= 1.0f; // ĐK va chạm
	delete e;
	return res;
}

void CSimon::colisionItem(CItem *it)
{
	switch (it->item)
	{
	case ItemType::BIG_HEART:
		DebugOut(L"[COLISION] chạm vào tim to bự nè: %d\n");
		break;
	case ItemType::WHIP:
		SetState(SIMON_STATE_LEVEL_UP);
		break;
	case ItemType::KNIFE:
		DebugOut(L"[COLISION] chạm vào KNIFE: %d\n");
		typeSubWeapon = WeaponType::KNIFE_WEAPON;
		break;
	case ItemType::AXE:
		typeSubWeapon = WeaponType::AXE_WEAPON;
		break;
	case ItemType::HOLY_WATER:
		typeSubWeapon = WeaponType::HOLY_WATER_WEAPON;
		break;
	case ItemType::STOP_WATCH:
		typeSubWeapon = WeaponType::STOP_WATCH_WEAPON;
		break;
	default:
		break;
	}
}

void CSimon::autoGotoX(float x)
{
	destinationX = x;
	isAutoGoX = true;
	accuTime = 0;
	nx = (destinationX - this->x > 0) ? 1 : -1;
	vx = SIMON_AUTO_GO_SPEED * nx;
}

void CSimon::attack()
{
	if ((CGame::GetInstance()->IsKeyDown(DIK_UP) && typeSubWeapon != WeaponType::NONE && subWeapon->isFlying)) return;
	else if (CGame::GetInstance()->IsKeyDown(DIK_UP) && typeSubWeapon != WeaponType::NONE && !subWeapon->isFlying)
	{
		isUseSubWeapon = true;
		subWeapon->isFlying = true;
		subWeapon->nx = nx;
		subWeapon->SetPosition(x, y);
		subWeapon->SetState(typeSubWeapon);
	}
	else
	{
		isUseSubWeapon = false;
	}
	if (!isJumping) vx = 0;
	isAttacking = true;
}

void CSimon::LoadResources()
{
	//xếp thứ tự add animation phải trùng với cái define ko là toi
	AddAnimation(100);		// idle right
	AddAnimation(101);		// idle left
	AddAnimation(102);		// sit right
	AddAnimation(103);		// sit left
	AddAnimation(104);		// jump right
	AddAnimation(105);		// jump left
	AddAnimation(106);		// walk right 
	AddAnimation(107);		// walk left 
	AddAnimation(108);		// ATTACK right
	AddAnimation(109);		// ATTACK left
	AddAnimation(110);		// SIT ATTACK right
	AddAnimation(111);		// SIT ATTACK left
	AddAnimation(113);		// HURT right
	AddAnimation(114);		// HURT left

	//AddAnimation(113);		// UP STAIR RIGHT
	//AddAnimation(114);		// UP STAIR LEFT
	//AddAnimation(115);		// IDLE UP STAIR RIGHT
	//AddAnimation(116);		// IDLE UP STAIR LEFT


	AddAnimation(112);		// die
}
