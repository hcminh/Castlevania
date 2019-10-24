#include "Whip.h"
#include "Textures.h"
#include "Simon.h"

CWhip::CWhip() : CGameObject()
{
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (nx > 0) setPosition(CSimon::GetInstance()->x - 24, CSimon::GetInstance()->y - 2);//-24 là trừ cái vị trí từ giữa con simon ra cái tay của nó lúc đưa ra sau (quay phải) quay trái thì trừ thêm -54
	else setPosition(CSimon::GetInstance()->x - 54 - 24, CSimon::GetInstance()->y - 2); // trừ y đi 2 vì sai số giữa 2 cái sprite

	if (animations[ani]->getCurrentFrame() == 2)
	{	
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
			}


		}
	}
}

void CWhip::Render()
{
	ani = level + nx + ((nx > 0) ? 1 : 0);
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (level == LEVEL_MAX)
		color = D3DCOLOR_ARGB(255, rand() % 255 + 200, rand() % 255 + 200, rand() % 255 + 200);
	animations[ani]->Render(x, y, color);
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if(nx == 1)
	{
		left = x + RANGE_OF_WHIP_SIMON_X + 32; //32 là khoảng cách từ x simon (biên bên trái) tới vị trí tay của simon (biên bên phải)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH); //cái này kiểm tra nếu level của whip >= 3 (Long chain) thì witdh dài hơn, chỉ kiểm tra 2 trường hợp vì short chain = normal whip
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else if(nx == -1)
	{
		left = x + RANGE_OF_WHIP_SIMON_X - 7 - ((level >= 3) ? RANGE_OF_LONG_NORMAL : 0); //7 là sai số khi dịch x lại gần biên bên trái của simon (vì nó chìa cái tay ra nữa)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH);
		bottom = top + WHIP_BBOX_HEIGHT;
	}
}

void CWhip::setPosition(float x, float y)
{
	this->x = x ; 
	this->y = y;
}

bool CWhip::isCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void CWhip::LoadResources()
{
	if (loadedSrc) return;
	loadedSrc = true;

	AddAnimation(151);		// normal whip left
	AddAnimation(153);		// short whip left
	AddAnimation(155);		// long whip left
	//xếp như này để chọn cách render whip là ani = level (từ 1 đến 3) + nx(-1 hoặc 1) + (nếu nx = 1, thì cộng thêm 1)
	AddAnimation(150);		// normal whip right
	AddAnimation(152);		// short whip right
	AddAnimation(154);		// long whip right

}



