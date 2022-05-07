#pragma once
#include<SDL.h>
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)
#define CAR_WIDTH (28)
#define CAR_HEIGHT (44)
#define BALL_RADIUS (60)
#define GOAL_WIDTH (50)
#define GOAL_HEIGHT (120)
typedef struct
{
	float x, y;//Tọa độ
	float dx, dy;//Vector vận tốc
	float gdx, gdy;//Vector lực
	float ang;//Góc
} Opject;


typedef struct
{
	SDL_Texture* texture;	// hỉnh ảnh 
	SDL_Rect drc;			// vi tri xuất item lên màn hình 
	int car1_touch;			// xe1 nhận item 
	int car2_touch;			// xe2 nhận item 
	int touch_time;			// thời gian nhân item 
} itemOpject;
