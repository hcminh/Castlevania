#include "Ground.h"
#include "Scenes.h"

CGround::CGround(int state, int width, int height, float x, float y)
{
	type = ObjectType::GROUND;
	isEnable = true;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->state = state;
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

