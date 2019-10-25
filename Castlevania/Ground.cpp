#include "Ground.h"
#include "Scenes.h"

CGround::CGround(float x, float y)
{
	type = ObjectType::GROUND;
	isEnable = true;
	SetPosition(x, y);
	LoadResources();
}

void CGround::Render()
{
	if(CScenes::GetInstance()->getCurrentScene() == SCENE_1)
		animations[0]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	if(CScenes::GetInstance()->getCurrentScene() == SCENE_2)
		animations[1]->Render(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
	//RenderBoundingBox();
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

	AddAnimation(350);
	AddAnimation(351);
}
