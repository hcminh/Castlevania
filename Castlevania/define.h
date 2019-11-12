#pragma once

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define MAX_FRAME_RATE 120


//định nghĩa load file index.txt
/*
-1 bbox
0 simon
1 whip
2 subweapon
3 item
4 enemy
*/
//define id để loadfile
#define ID_BBOX		     -1
#define ID_SCENE		 1
#define ID_MAP		     2
#define ID_OBJECT	     3

//
#define OBJECT_SCENE_1 -999
#define ID_SIMON	0
#define ID_WHIP		1
#define ID_WEAPON	2 
#define ID_ITEM		3
#define ID_GROUND	4
#define ID_DOOR		5
#define ID_Enemy	6

//SIMON DÙNG ANI CỦA SỐ TỪ 100 đến 112, sprite từ 10001 đến 10023 (lại dư thêm tới 10050)
//WHIP DÙNG ANI CỦA SỐ TỪ 150 đến 155, sprite từ 10051 đến 10069 (lại dư thêm tới 10100)
//SubWEAPON DÙNG ANI CỦA SỐ TỪ 200 đến 250, sprite từ 10051 đến 10101 (lại dư thêm tới 10150)
//ITEM DÙNG ANI CỦA SỐ TỪ 250 đến 300, sprite từ 10151 đến 10173 (lại dư thêm tới 10200)
//ENEMY dùng ANI CỦA SỐ TỪ 300 đến 350, sprite từ 10201 đến  (lại dư thêm tới 10250)


#define TILE_WIDTH		32
#define TILE_HEIGHT		32

#define MAP_MIN_COL	0

// Scene
//SCENE_1 sprite từ 1001 đến 1999
//SCENE_2 sprite từ 2001 đến 2999
//SCENE_3 sprite từ 3001 đến 3999

#define SCENE_1			1000
#define SCENE_2			2000
#define SCENE_3			3000

//layout
#define LAYOUT_HEIGHT 80