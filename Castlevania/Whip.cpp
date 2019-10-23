#include "Whip.h"
#include "Textures.h"

//#include "Scenes.h"

CWhip::CWhip() : CGameObject()
{
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
	if (direct < 0) return;
	ani = level + direct; // level bang gia tri cua loai roi, direct = -1 -> ko danh, direct = 0 danh phai, direct = 1 danh trai
	animations[ani]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if(direct == 0)
	{
		left = x + RANGE_OF_WHIP_SIMON_X + 32; //32 là khoảng cách từ x simon (biên bên trái) tới vị trí tay của simon (biên bên phải)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH); //cái này kiểm tra nếu level của whip >= 3 (Long chain) thì witdh dài hơn, chỉ kiểm tra 2 trường hợp vì short chain = normal whip
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else if(direct == 1)
	{
		left = x + RANGE_OF_WHIP_SIMON_X - 7 - ((level >= 3) ? RANGE_OF_LONG_NORMAL : 0); //7 là sai số khi dịch x lại gần biên bên trái của simon (vì nó chìa cái tay ra nữa)
		top = y + 15; //từ đầu xuống tay của con simon là 15
		right = left + ((level >= 3) ? LONG_CHAIN_BBOX_WIDTH : WHIP_BBOX_WIDTH);
		bottom = top + WHIP_BBOX_HEIGHT;
	}
}

void CWhip::setPosition(float x, float y, int direct)
{
	this->direct = direct;
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

	AddAnimation(150);		// normal whip right
	AddAnimation(151);		// normal whip left

	AddAnimation(152);		// short whip right
	AddAnimation(153);		// short whip left

	AddAnimation(154);		// long whip right
	AddAnimation(155);		// long whip left
}



