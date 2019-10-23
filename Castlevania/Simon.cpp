#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include <math.h>
//#include "Goomba.h"


CSimon * CSimon::__instance = NULL;

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	accuTime += dt;
	if (unTouch && accuTime <= SIMON_UP_LEVEL_TIME) {
		return;
	} 
	unTouch = false;

	// Calculate dx, dy 
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);
	if(subWeapon != NULL) subWeapon->Update(dt, coObjects);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		/*untouchable_start = 0;
		untouchable = 0;*/
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

		if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = 0;
			isJumping = false;
		}
		// Collision logic with Goombas
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];
		//	if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
		//	{
		//		CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
		//		// jump on top >> kill Goomba and deflect a bit 
		//		if (e->ny < 0)
		//		{
		//			if (goomba->GetState() != GOOMBA_STATE_DIE)
		//			{
		//				goomba->SetState(GOOMBA_STATE_DIE);
		//				vy = -SIMON_JUMP_DEFLECT_SPEED;
		//			}
		//		}
		//		else if (e->nx != 0)
		//		{
		//			if (untouchable == 0)
		//			{
		//				if (goomba->GetState() != GOOMBA_STATE_DIE)
		//				{
		//						SetState(SIMON_STATE_DIE);
		//				}
		//			}
		//		}
		//	}
		//}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	int ani;
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_DIE;
	}
	else if (unTouch) {
		if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
		else ani = SIMON_ANI_IDLE_LEFT;
		color = D3DCOLOR_ARGB(255, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
	}
	else if (isAttacking && isJumping)
	{
		if (nx > 0) {
			ani = SIMON_ANI_ATTACK_RIGHT;
			whip->setPosition(x - 24, y - 3, 0);//-24 là trừ cái vị trí từ giữa con simon ra cái tay của nó lúc đưa ra sau (quay phải) quay trái thì trừ thêm -54
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 0);
		}
		else
		{
			ani = SIMON_ANI_ATTACK_LEFT;
			whip->setPosition(x - 54 - 24, y - 3, 1); //khi nao ngoi xuong thi y - 3 vi xuong bi hut px
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 1);
		}
	}
	else if (isJumping)
	{
		if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
		else ani = SIMON_ANI_JUMP_LEFT;
	}
	else if (isAttacking && isSitting)
	{
		if (nx > 0) {
			ani = SIMON_ANI_SIT_ATTACK_RIGHT;
			whip->setPosition(x - 24, y -3, 0);//-24 là trừ cái vị trí từ giữa con simon ra cái tay của nó lúc đưa ra sau (quay phải) quay trái thì trừ thêm -54
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 0);
		}
		else
		{
			ani = SIMON_ANI_SIT_ATTACK_LEFT;
			whip->setPosition(x - 54 - 24, y - 3, 1); //khi nao ngoi xuong thi y - 3 vi xuong bi hut px
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 1);
		}
	}
	else if (isAttackWithSub)
	{
		if (nx > 0) {
			ani = SIMON_ANI_ATTACK_RIGHT;
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 0);
		} 
		else
		{
			ani = SIMON_ANI_ATTACK_LEFT;
			if (subWeapon != NULL) subWeapon->setPosition(x, y, 1);
		}
	}
	else if (isAttacking)
	{
		if (nx > 0) {
			ani = SIMON_ANI_ATTACK_RIGHT;
			whip->setPosition(x - 24, y, 0); //direct = 0 khi danh ben phai
		}
		else
		{
			ani = SIMON_ANI_ATTACK_LEFT;
			whip->setPosition(x - 54 - 24, y, 1);
		}
	}
	else if (isSitting)
	{
		if (nx > 0) ani = SIMON_ANI_SIT_RIGHT;
		else ani = SIMON_ANI_SIT_LEFT;
	}
	else 
	{
			if (vx == 0)
			{
				if (state == SIMON_STATE_SIT) {
					if (nx > 0) ani = SIMON_ANI_SIT_RIGHT;
					else ani = SIMON_ANI_SIT_LEFT;
			}
				else
				{
					if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
					else ani = SIMON_ANI_IDLE_LEFT;
				}
			}
			else if (vx > 0)
				ani = SIMON_ANI_WALKING_RIGHT;
			else ani = SIMON_ANI_WALKING_LEFT;
	}

	animations[ani]->Render(x, y, color);

	if (subWeapon != NULL) subWeapon->Render();
	whip->Render();

	if (isAttacking && animations[ani]->getCurrentFrame() >= MAX_ATTACK_FRAME)
	{
		if (subWeapon != NULL) subWeapon->isFlying = true;
		isAttacking = false;
		isAttackWithSub = false;
		whip->setPosition(x - 54 - 24, y - 3, -1); //dat direct gia tri bang -1 de ko danh
	}

	//RenderBoundingBox();
}

CSimon *CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	return __instance;
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	//if (isAttacking) return;
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		if (isSitting) vx = 0;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		if (isSitting) vx = 0;
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
		y -= 18;
		vx = 0;
		break;
	case SIMON_STATE_JUMP:
		if (isJumping)return;
		isJumping = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;

	case SIMON_STATE_ATTACK:
		if (isAttacking)return;
		isAttacking = true;
		break;
	case SIMON_STATE_ATTACK_WITH_SUB:
		if (isAttacking && isAttackWithSub) return;
		isAttackWithSub = true;
		isAttacking = true;
		break;
	case SIMON_STATE_IDLE:
		//unTouch = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_LEVEL_UP:
		if (unTouch) return;
		whip->levelUp();
		unTouch = true;
		accuTime = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (nx > 0)
	{
		if (isSitting)
		{
			left = x + 16;
			top = y;
			right = x + SIMON_BBOX_WIDTH - 13;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		}
		/*else if (isJumping)
		{
			left = x;
			top = y;
			right = x + SIMON_BBOX_WIDTH;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		}*/
		else
		{
			left = x + 16;
			top = y;
			right = x + SIMON_BBOX_WIDTH - 14;
			bottom = top + SIMON_BBOX_HEIGHT;
		}
	}
	else {
		if (isSitting)
		{
			left = x + 13;
			top = y;
			right = x + SIMON_BBOX_WIDTH -16;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		}
		/*else if (isJumping)
		{
			left = x;
			top = y;
			right = x + SIMON_BBOX_WIDTH;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		}*/
		else
		{
			left = x + 13;
			top = y;
			right = x + SIMON_BBOX_WIDTH - 16;
			bottom = top + SIMON_BBOX_HEIGHT;
		}
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

void CSimon::colisionItem(CItem *item)
{
	switch (item->item)
	{
	case ItemType::BIG_HEART:
		DebugOut(L"[COLISION] chạm vào tim to bự nè: %d\n");
		break;
	case ItemType::WHIP:
		SetState(SIMON_STATE_LEVEL_UP);
		break;
	case ItemType::KNIFE:
		subWeapon = new CWeapon();
		subWeapon->LoadResources();
		break;
	default:
		break;
	}
}

void CSimon::LoadResources()
{
	if (loadedSrc) return;
	loadedSrc = true;
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
	AddAnimation(112);		// die
}
