#pragma once

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define MAX_FRAME_RATE 120

#define ID_TEX_BBOX		-1
#define ID_TEX_SIMON	0
#define ID_TEX_WHIP		1
#define ID_TEX_WEAPON	2
#define ID_TEX_CANDLE	3
#define ID_TEX_ITEM		4
#define ID_TEX_GROUND	5

//define id của obj
#define OBJECT_SCENE_1 -999
#define ID_SIMON	0
#define ID_WHIP		1
#define ID_WEAPON	2 
#define ID_ITEM		3
#define ID_GROUND	4
#define ID_DOOR		5

//SIMON DÙNG ANI CỦA SỐ TỪ 100 đến 112, sprite từ 10001 đến 10023 (lại dư thêm tới 10050)
//WHIP DÙNG ANI CỦA SỐ TỪ 150 đến 155, sprite từ 10051 đến 10069 (lại dư thêm tới 10100)
//SubWEAPON DÙNG ANI CỦA SỐ TỪ 200 đến 250, sprite từ 10051 đến 10101 (lại dư thêm tới 10150)
//CANDLE DÙNG ANI CỦA SỐ TỪ 250 đến 300, sprite từ 10151 đến 10155 (lại dư thêm tới 10200)
//ITEM DÙNG ANI CỦA SỐ TỪ 300 đến 350, sprite từ 10201 đến 10155 (lại dư thêm tới 10250)
//GROUND DÙNG ANI CỦA SỐ TỪ 350 đến 400, sprite từ 10251 đến 10251 (lại dư thêm tới 10300)