#pragma once
#include<SDL.h>

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (800)
#define CAR_WIDTH (14)
#define CAR_HEIGHT (22)
#define BALL_RADIUS (100)

typedef struct
{
	float x, y;
	float dx, dy;
	float gdx, gdy;
	float ang;
} Opject;

