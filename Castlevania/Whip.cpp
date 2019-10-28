#include "Whip.h"
#include "Textures.h"
#include "Simon.h"

CWhip::CWhip(float &x, float &y, int &nx) : CGameObject()
{
	xSimon = &x;
	ySimon = &y;
	nxSimon = &nx;
	LoadResources();
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (animations[ani]->getCurrentFrame() == 2)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			float top, left, bottom, right;
			coObjects->at(i)->GetBoundingBox(left, top, right, bottom);
			if (isCollision(left, top, right, bottom))
			{
				if (coObjects->at(i)->type == ObjectType::ITEM);
				{
					coObjects->at(i)->SetState(CANDLE_STATE_DESTROYING);
				}
			}


		}
	}
}

void CWhip::Render()
{
	ani = level + *nxSimon + ((*nxSimon > 0) ? 1 : 0);
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (level == LEVEL_MAX)
		color = D3DCOLOR_ARGB(255, rand() % 255 + 200, rand() % 255 + 200, rand() % 255 + 200);
	animations[ani]->Render(*xSimon - 80, *ySimon, color);
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (level < LEVEL_MAX)
	{
		left = *xSimon + X_SIMON_TO_HAND*(*nxSimon);
		top = *ySimon + Y_SIMON_TO_HAND;
		right = left + WHIP_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else
	{
		left = *xSimon + (((*nxSimon) > 0) ? X_SIMON_TO_HAND : X_SIMON_TO_HAND_LONG) * (*nxSimon);
		top = *ySimon + Y_SIMON_TO_HAND;
		right = left + LONG_CHAIN_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
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



