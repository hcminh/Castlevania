#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include <math.h>

CSimon * CSimon::__instance = NULL;
CSimon *CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	return __instance;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//upgrade whip level
	accuTime += dt;
	if (levelUpgrade && accuTime <= SIMON_UP_LEVEL_TIME) {
		return;
	}
	levelUpgrade = false;

	// Calculate dx, dy 
	CGameObject::Update(dt);

	//go to X, when touch door
	if (isAutoGoX && (x > destinationX + 5 || x < destinationX - 5))
	{
		x += dx;
		return;
	}
	isAutoGoX = false;

	whip->Update(dt, coObjects);
	if (subWeapon != NULL) subWeapon->Update(dt, coObjects);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}

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

		if (nx != 0) 
		{
			vx = 0;
			//Collision logic with DOOR
			//for (UINT i = 0; i < coEventsResult.size(); i++)
			//{
			//	LPCOLLISIONEVENT e = coEventsResult[i];
			//	if (e->obj->type == ObjectType::DOOR) // nếu e->obj là DOOR
			//	{
			//		if(x + SIMON_BBOX_WIDTH < 1378.0)
			//			autoGotoX(1385.0);
			//		else if(x >= 1385.0)
			//		{
			//			autoGotoX(1250.0);
			//			//autoGotoX(1385.0);
			//		}
			//		//xử lý qua màn
			//	}
			//}
		}
		if (ny != 0) {
			vy = 0;
			if (isJumping) {
				/*isSitting = false;
				isJumping = false;
				y -= 18;
				vx = 0;*/
				SetState(SIMON_STATE_STANDUP);
				isJumping = false;
			}
		}
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
	else if (isAttacking && isSitting)  ani = SIMON_ANI_SIT_ATTACK_RIGHT;
	else if (isAttacking)				ani = SIMON_ANI_ATTACK_RIGHT;
	else if (isJumping)					ani = SIMON_ANI_JUMP_RIGHT;
	else if (isSitting)					ani = SIMON_ANI_SIT_RIGHT;
	else if (vx != 0) ani = SIMON_ANI_WALKING_RIGHT;

	ani += (nx > 0) ? 0 : 1; //vì hành động phải chỉ cách hành động trái 1 frame nên sét ani bằng phải rồi kiểm tra nx là dc
	animations[ani]->Render(x, y, color);

	//xử lý render sau khi đã bấm nút
	if (subWeapon != NULL && subWeapon->isFlying) subWeapon->Render();

	//xử lý render vũ khí khi vừa bấm nút
	if (isAttacking)
	{
		if (isUseSubWeapon)
		{
			subWeapon->isFlying = true;
			subWeapon->Render();
		}
		else whip->Render();
	}
	//nếu là frame đánh cuói cùng thì tắt isAttacking
	if (isAttacking && animations[ani]->getCurrentFrame() >= MAX_ATTACK_FRAME)
	{
		isAttacking = false;
	}
	//RenderBoundingBox();
}

//void CSimon::Render()
//{
//	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
//	if (state == SIMON_STATE_DIE) ani = SIMON_ANI_DIE;
//	else if (levelUpgrade) {
//		if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
//		else ani = SIMON_ANI_IDLE_LEFT;
//		color = D3DCOLOR_ARGB(255, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
//	}
//	else if (isAttacking && isSitting)
//	{
//		if (nx > 0) ani = SIMON_ANI_SIT_ATTACK_RIGHT;
//		else ani = SIMON_ANI_SIT_ATTACK_LEFT;
//	}
//	else if (isAttacking)
//	{
//		if (nx > 0) ani = SIMON_ANI_ATTACK_RIGHT;
//		else ani = SIMON_ANI_ATTACK_LEFT;
//	}
//	else if (isJumping)
//	{
//		if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
//		else ani = SIMON_ANI_JUMP_LEFT;
//	}
//	else if (isSitting)
//	{
//		if (nx > 0) ani = SIMON_ANI_SIT_RIGHT;
//		else ani = SIMON_ANI_SIT_LEFT;
//	}
//	else
//	{
//		if (vx == 0)
//		{
//			if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
//			else ani = SIMON_ANI_IDLE_LEFT;
//		}
//		else if (vx > 0)
//			ani = SIMON_ANI_WALKING_RIGHT;
//		else ani = SIMON_ANI_WALKING_LEFT;
//	}
//
//	animations[ani]->Render(x, y, color);
//
//	//xử lý render sau khi đã bấm nút
//	if (subWeapon != NULL) subWeapon->Render();
//
//	//xử lý render vũ khí khi vừa bấm nút
//	if (isAttacking)
//	{
//		if (subWeapon != NULL && isUseSubWeapon)
//		{
//			subWeapon->isFlying = true;
//			subWeapon->Render();
//		}
//		else if (!isUseSubWeapon) whip->Render();
//	}
//	//nếu là frame đánh cuói cùng thì tắt isAttacking
//	if (isAttacking && animations[ani]->getCurrentFrame() >= MAX_ATTACK_FRAME)
//	{
//		isAttacking = false;
//	}
//	RenderBoundingBox();
//}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	if (isAttacking || isAutoGoX) return;
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
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_LEVEL_UP:
		if (levelUpgrade) return;
		whip->levelUp();
		levelUpgrade = true;
		accuTime = 0;
		break;
	case SIMON_STATE_AUTO_GO:
		if (isAutoGoX) return;
		autoGotoX(1385.0);
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
		if (isSitting || isJumping)
		{
			left = x + 16;
			top = y;
			right = x + SIMON_BBOX_WIDTH - 14;
			bottom = top + SIMON_SIT_BBOX_HEIGHT;
		} 
		else
		{
			left = x + 16;
			top = y;
			right = x + SIMON_BBOX_WIDTH - 14;
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
	if (CGame::GetInstance()->IsKeyDown(DIK_UP) && subWeapon != NULL && subWeapon->isFlying) return;
	else if (CGame::GetInstance()->IsKeyDown(DIK_UP) && subWeapon != NULL && !subWeapon->isFlying)
	{
		isUseSubWeapon = true;
		subWeapon->isFlying = true;
		subWeapon->nx = nx;
		subWeapon->setPosition(x, y);
	}
	else
	{
		isUseSubWeapon = false;
		whip->nx = nx;
		if (nx > 0) whip->setPosition(x - 24, y - 2);//-24 là trừ cái vị trí từ giữa con simon ra cái tay của nó lúc đưa ra sau (quay phải) quay trái thì trừ thêm -54
		else whip->setPosition(x - 54 - 24, y - 2); // trừ y đi 2 vì sai số giữa 2 cái sprite
	}
	if (!isJumping) vx = 0;
	isAttacking = true;
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
