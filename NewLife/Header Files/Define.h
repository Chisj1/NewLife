#pragma once
#include<SDL.h>
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)
#define CAR_WIDTH (28)
#define CAR_HEIGHT (44)
#define BALL_RADIUS (60)

typedef struct
{
	float x, y;
	float dx, dy;
	float gdx, gdy;
	float ang;
} Opject;

