#include "Ground.h"
#include "Scenes.h"

CGround::CGround(float x, float y, int width, int height)
{
	type = ObjectType::GROUND;
	isEnable = true;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CGround::Render()
{
	//RenderBoundingBox();
}

void CGround::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

