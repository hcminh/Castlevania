#include <algorithm>
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
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Update vy
	if (isOnStair == false && isAutoWalk == false)
	{
		vy += SIMON_GRAVITY * dt;
	}

	// Auto - walk
	if (isAutoWalk)
		DoAutoWalk();

	updateState();

	if (coObjects == NULL)
	{
		if (isAutoWalk == false)
		{
			x += dx;
			y += dy;
		}

		return;
	}

	vector<LPGAMEOBJECT> listObject; // lọc danh sách có khả năng va chạm
	listObject.clear();
	for (UINT i = 0; i < coObjects->size(); i++) {
		if (coObjects->at(i)->type != ObjectType::ITEM && coObjects->at(i)->type != ObjectType::STAIR)
			listObject.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(&listObject, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0 && isAutoWalk == false)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		if (isAutoWalk == false)
		{
			x += min_tx * dx + nx * 0.4f;	// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->type == ObjectType::GROUND)
			{
				if (e->ny < 0)
				{
					vy = 0;
					DebugOut(L"[HAIZZZZZZZZZZZ] \n");
					if (isJumping) SetState(SIMON_STATE_STANDUP);
				}
				else if (e->ny > 0)
				{
					y += dy;
				}

				// Khi đang lên/xuống cầu thang, va chạm theo trục x sẽ không được xét tới
				if (state == SIMON_STATE_UP_STAIR || state == SIMON_STATE_DOWN_STAIR)
				{
					if (nx != 0) x -= nx * 0.1f;
					//y -= 5;
				}
			}
			else if (e->obj->type == ObjectType::DOOR)
			{

				CScenes::GetInstance()->changeScene();
			}
			else if (e->obj->type == ObjectType::ENEMY)
			{
				if (!invisible && !untouchable)
				{
					SetState(SIMON_STATE_ATTACKED);
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}

				//Sử lý nếu đang trên cầu thang thì ko bị nhảy ra sau
			}
		}
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

void CSimon::updateState()
{
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
		SetState(SIMON_STATE_SIT_AFTER_FALL);
	}
	//sitting after falling
	if (isFalling && (GetTickCount() - sittingStart > SIMON_SIT_AFTER_FALL_TIME))
	{
		sittingStart = 0;
		SetState(SIMON_STATE_STANDUP);
	}
	//reset untouchable timer if untouchable time has passed
	if (untouchable && (GetTickCount() - untouchableStart > SIMON_UNTOUCHABLE_TIME))
	{
		untouchableStart = 0;
		untouchable = false;
	}
	//reset untouchable timer if untouchable time has passed
	if (invisible && (GetTickCount() - invisibleStart > SIMON_INVISIABLE_TIME))
	{
		invisibleStart = 0;
		invisible = false;
	}
}

void CSimon::Render()
{
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (state == SIMON_STATE_DIE) ani = ANI_DIE_RIGHT;
	if (isOnStair)
	{
		ani = ANI_IDLE_UP_STAIR_RIGHT;
		//if (state == SIMON_STATE_UP_STAIR) ani = ANI_UP_STAIR_RIGHT;
	}
	else
	{
		ani = ANI_IDLE_RIGHT;
		if (levelUpgrade) color = D3DCOLOR_ARGB(255, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
		else if (isHurting) ani = ANI_HURT_RIGHT;
		else if (isAttacking && isSitting)  ani = ANI_SIT_ATTACK_RIGHT;
		else if (isAttacking)				ani = ANI_ATTACK_RIGHT;
		else if (isJumping)					ani = ANI_JUMP_RIGHT;
		else if (isSitting)					ani = ANI_SIT_RIGHT;
		else if (vx != 0) ani = ANI_WALKING_RIGHT;
	}

	if (untouchable) color = D3DCOLOR_ARGB(rand() % 220 + 100, 255, 255, 255);
	else if (invisible) color = D3DCOLOR_ARGB(150, 255, 255, 255);
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
	case SIMON_STATE_SIT_AFTER_FALL:
		vy = 0;
		startSittingAfterFall();
		break;
	case SIMON_STATE_STANDUP:
		isFalling = false;
		isSitting = false;
		isJumping = false;
		y -= 18;
		vx = 0;
		vy = 0;
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
		isAutoWalk = false;
		isOnStair = false;
		isUpStair = false;
		isDownStair = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_LEVEL_UP:
		if (levelUpgrade) return;
		whip->levelUp();
		vx = 0;
		startLevelUp();
		break;
	case SIMON_STATE_AUTO_GO:
		if (isAutoGoX) return;
		autoGotoX(1385.0);
		break;
	case SIMON_STATE_ATTACKED:
		isJumping = true;
		vx = -0.1*nx;
		vy = -0.4f;
		startHurting();
		startUntouchable();
		break;
	case SIMON_STATE_INVISIBLE:
		startInvisible();
		break;
	case SIMON_STATE_DOWN_STAIR:
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = SIMON_STAIR_SPEED_Y;
		isOnStair = true;
		isUpStair = false;
		isDownStair = true;
		break;
	case SIMON_STATE_UP_STAIR:
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		isOnStair = true;
		isUpStair = true;
		isDownStair = false;
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
		break;
	case ItemType::WHIP:
		SetState(SIMON_STATE_LEVEL_UP);
		break;
	case ItemType::KNIFE:
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
	case ItemType::INVISIBLE:
		SetState(SIMON_STATE_INVISIBLE);
		break;
	default:
		break;
	}
}

bool CSimon::isColisionEnemy(CEnemy * enemy)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);  // lấy BBOX của simon

	enemy->GetBoundingBox(l1, t1, r1, b1);
	if (CGameObject::AABB(l, t, r, b, l1, t1, r1, b1))
	{
		return true; // check with AABB
	}
	LPCOLLISIONEVENT e = SweptAABBEx(enemy); // kt va chạm giữa 2 object bằng sweptAABB
	bool res = e->t > 0 && e->t <= 1.0f; // ĐK va chạm
	delete e;
	return res;
}

void CSimon::colisionEnemy(CEnemy * enemy)
{
}

bool CSimon::isColisionWeapon(CWeapon *weapon)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);  // lấy BBOX của simon

	weapon->GetBoundingBox(l1, t1, r1, b1);
	if (CGameObject::AABB(l, t, r, b, l1, t1, r1, b1))
	{
		return true; // check with AABB
	}
	LPCOLLISIONEVENT e = SweptAABBEx(weapon); // kt va chạm giữa 2 object bằng sweptAABB
	bool res = e->t > 0 && e->t <= 1.0f; // ĐK va chạm
	delete e;
	return res;
}

void CSimon::colisionWeapon(CWeapon *weapon)
{
}

bool CSimon::isColisionStair(vector<LPGAMEOBJECT> stairs)
{
	canMoveUpStair = false;
	canMoveDownStair = false;

	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_t += 50;
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống

	for (UINT i = 0; i < stairs.size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		stairs[i]->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);


		if (CGameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			if (stairs[i]->GetState() == 0) stairDirection = 1;
			else stairDirection = -1;

			stairCollided = stairs[i];

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			if (simon_b < stair_b) canMoveDownStair = true;
			if (y >= stair_t - 35) canMoveUpStair = true;

			// kiểm tra xem simon có thể di chuyển lên/xuống hay ko
			// (dựa vào toạ độ của 2 bậc liền kề hơn/kém nhau 32)

			float upstair_x = -999, upstair_y = -999; // toạ độ của bậc thang liền kề

			for (UINT j = 0; j < stairs.size(); j++)
			{
				if (i == j)
					continue;

				stairs[j]->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == 32 && dy == -32) // vì bậc nằm trên nên dy = -...
					canMoveUpStair = true;

				if (dx == 32 && dy == 32) // vì bậc nằm duoi nên dy = +...
					canMoveDownStair = true;
			}


			return true; // collision between Simon and stairs
		}

	}

	return false;
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
	AddAnimation(115);		// Đứng lên Phải
	AddAnimation(116);		// Đứng lên Trái
	AddAnimation(117);		// Đi lên Phải
	AddAnimation(118);		// Đi lên Trái
	AddAnimation(119);		// Đánh lên phải
	AddAnimation(120);		// Đánh lên trái
	AddAnimation(121);		// Đứng Xuống Phải
	AddAnimation(122);		// Đứng Xuống Trái
	AddAnimation(123);		// Đi Xuống Phải
	AddAnimation(124);		// Đi Xuống Trái
	AddAnimation(125);		// Đánh Xuống phải
	AddAnimation(126);		// Đánh Xuống trái

	AddAnimation(112);		// die
	AddAnimation(127);		// die
}

void CSimon::AutoWalk(float distance, int new_state, int new_nx)
{
	isAutoWalk = true;

	autoWalkDistance = distance;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}

void CSimon::DoAutoWalk()
{
	if (abs(dx) <= abs(autoWalkDistance))
	{
		x += dx;
		y += dy;
		autoWalkDistance -= dx;
	}
	else
	{
		x += autoWalkDistance;
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;

		SetState(state);
		if (state == SIMON_STATE_DOWN_STAIR) y += 1.0f; // + 1.0f để đảm bảo simon sẽ va chạm với bậc thang trong lần update kế tiếp

		isAutoWalk = false;
		autoWalkDistance = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
}
