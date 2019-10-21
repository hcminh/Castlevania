﻿#include "Brick.h"

CBrick::CBrick()
{
	type = ObjectType::BRICK;
}

void CBrick::Render()
{
	animations[0]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
}


void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}