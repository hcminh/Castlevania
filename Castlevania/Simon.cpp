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
	int ani = this->currentFrame;
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_DIE;
	}
	else if (unTouch) {
		//ani = SIMON_ANI_LEVEL_UP;
		color = D3DCOLOR_ARGB(255, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
	}
	else if (isAttacking && isJumping)
	{
		if (nx > 0) {
			ani = SIMON_ANI_ATTACK_RIGHT;
			whip->setPosition(x - 24, y - 3, 0);//-24 là trừ cái vị trí từ giữa con simon ra cái tay của nó lúc đưa ra sau (quay phải) quay trái thì trừ thêm -54
		}
		else
		{
			ani = SIMON_ANI_ATTACK_LEFT;
			whip->setPosition(x - 54 - 24, y - 3, 1); //khi nao ngoi xuong thi y - 3 vi xuong bi hut px
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
		}
		else
		{
			ani = SIMON_ANI_SIT_ATTACK_LEFT;
			whip->setPosition(x - 54 - 24, y - 3, 1); //khi nao ngoi xuong thi y - 3 vi xuong bi hut px
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
	whip->Render();

	if (isAttacking && animations[ani]->getCurrentFrame() >= MAX_ATTACK_FRAME)
	{
		isAttacking = false;
		whip->setPosition(x - 54 - 24, y - 3, -1); //dat direct gia tri bang -1 de ko danh
	}

	RenderBoundingBox();
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
	//float l, t, r, b;
	//float l1, t1, r1, b1;
	//this->GetBoundingBox(l, t, r, b);  // lấy BBOX của simon

	//item->GetBoundingBox(l1, t1, r1, b1);
	//if (CGameObject::AABB(l, t, r, b, l1, t1, r1, b1))
	//{
	//	return true; // check with AABB
	//}
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
	default:
		break;
	}
}

void CSimon::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"textures\\full-simon.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	// big
	sprites->Add(10001, 900, 0, 900 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);		// idle right

	sprites->Add(10002, 840, 0, 840 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);		// walk
	sprites->Add(10003, 780, 0, 780 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10004, 720, 0, 720 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);

	sprites->Add(10011, 0, 0, 0 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);		// idle left

	sprites->Add(10012, 60, 0, 60 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);		// walk
	sprites->Add(10013, 120, 0, 120 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10014, 180, 0, 180 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);


	sprites->Add(10099, 215, 120, 215 + SIMON_SPRITE_WIDTH, 120 + SIMON_SPRITE_HEIGHT, texSimon);		// die 

	//// sit
	sprites->Add(10021, 600, 198 + 18, 600 + SIMON_SPRITE_WIDTH, 198 + SIMON_SPRITE_HEIGHT, texSimon);			// sit right

	sprites->Add(10031, 300, 198 + 18, 300 + SIMON_SPRITE_WIDTH, 198 + SIMON_SPRITE_HEIGHT, texSimon);			// sit left

	////// jump
	//sprites->Add(10041, 540, 198, 540 + SIMON_SPRITE_WIDTH, 198 + SIMON_SPRITE_HEIGHT - 18, texSimon);			// jump right

	//sprites->Add(10051, 360, 198, 360 + SIMON_SPRITE_WIDTH, 198 + SIMON_SPRITE_HEIGHT - 18, texSimon);			// jump left


	//// SITTING ATTACK 
	sprites->Add(10022, 480, 66 + 18, 480 + SIMON_SPRITE_WIDTH, 66 + SIMON_SPRITE_HEIGHT, texSimon);			// SIT ATTACK right
	sprites->Add(10023, 900, 132 + 18, 900 + SIMON_SPRITE_WIDTH, 132 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10024, 840, 132 + 18, 840 + SIMON_SPRITE_WIDTH, 132 + SIMON_SPRITE_HEIGHT, texSimon);

	sprites->Add(10032, 420, 66 + 18, 420 + SIMON_SPRITE_WIDTH, 66 + SIMON_SPRITE_HEIGHT, texSimon);			// SIT ATTACK left		
	sprites->Add(10033, 0, 132 + 18, 0 + SIMON_SPRITE_WIDTH, 132 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10034, 60, 132 + 18, 60 + SIMON_SPRITE_WIDTH, 132 + SIMON_SPRITE_HEIGHT, texSimon);

	//// ATTACK
	sprites->Add(10061, 300, 0, 300 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);			// ATTACK right
	sprites->Add(10062, 360, 0, 360 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10063, 420, 0, 420 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);

	sprites->Add(10064, 600, 0, 600 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);			// ATTACK left		
	sprites->Add(10065, 540, 0, 540 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);
	sprites->Add(10066, 480, 0, 480 + SIMON_SPRITE_WIDTH, 0 + SIMON_SPRITE_HEIGHT, texSimon);

	LPANIMATION ani;

	ani = new CAnimation(100);	// idle right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// sit right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// sit left
	ani->Add(10031);
	animations->Add(403, ani);

	ani = new CAnimation(100);	// jump right
	ani->Add(10021);
	animations->Add(404, ani);

	ani = new CAnimation(100);	// jump left
	ani->Add(10031);
	animations->Add(405, ani);

	ani = new CAnimation(100);	// walk right 
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// // walk left 
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(10014);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// // ATTACK right 
	ani->Add(10064);
	ani->Add(10065);
	ani->Add(10066);
	ani->Add(10001);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// // ATTACK left 
	ani->Add(10061);
	ani->Add(10062);
	ani->Add(10063);
	ani->Add(10011);
	animations->Add(503, ani);


	ani = new CAnimation(100);	// // SIT ATTACK right 
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	ani->Add(10021);
	animations->Add(504, ani);

	ani = new CAnimation(100);	// // SIT ATTACK left 
	ani->Add(10032);
	ani->Add(10033);
	ani->Add(10034);
	ani->Add(10031);
	animations->Add(505, ani);


	ani = new CAnimation(100);		// Simon die
	ani->Add(10099);
	animations->Add(599, ani);

	AddAnimation(400);		// idle right
	AddAnimation(401);		// idle left

	AddAnimation(402);		// sit right
	AddAnimation(403);		// sit left

	AddAnimation(500);		// walk right big
	AddAnimation(501);		// walk left big

	AddAnimation(404);		// jump right
	AddAnimation(405);		// jump left

	AddAnimation(599);		// die

	AddAnimation(502);		// ATTACK right
	AddAnimation(503);		// ATTACK left

	AddAnimation(504);		// SIT ATTACK right
	AddAnimation(505);		// SIT ATTACK left
}

void CSimon::Attack()
{

}