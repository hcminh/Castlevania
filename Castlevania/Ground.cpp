#include "Ground.h"
#include "Scenes.h"

CGround::CGround(int state, int isEnable, int width, int height, float x, float y)
{
	type = ObjectType::GROUND;
	this->isEnable = isEnable < 0 ? true : false; //set nguoc lai de khoi phai sua file lau
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

