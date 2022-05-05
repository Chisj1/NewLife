#pragma once
#include<SDL.h>
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)
#define CAR_WIDTH (28)
#define CAR_HEIGHT (44)
#define BALL_RADIUS (60)

typedef struct
{
	float x, y;//Tọa độ
	float dx, dy;//Vector vận tốc
	float gdx, gdy;//Vector lực
	float ang;//Góc
} Opject;

typedef struct 
{
	SDL_Texture* texture ;
	SDL_Rect src;
	SDL_Rect drc;
	int car1_touch;
	int car2_touch;
	int touch_time;
} itemOpject;
