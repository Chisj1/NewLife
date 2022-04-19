#pragma once
#include<SDL.h>
#include"Define.h"


int processEvents(SDL_Window *window, Opject *car, Opject *car2, Opject *ball);
void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball, SDL_Texture *background);
void initOpject(Opject *opjects, int posX, int posY);