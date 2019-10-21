#include "Ground.h"

CGround::CGround()
{
	type = ObjectType::GROUND;
	LoadResources();
}

void CGround::Render()
{
	animations[0]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	RenderBoundingBox();
}


void CGround::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + GROUND_BBOX;
	b = y + GROUND_BBOX;
}

void CGround::LoadResources()
{
	if (loadedSrc) return;
	loadedSrc = true;
	AddAnimation(601);
}
