#pragma once

#include<SDL.h>


SDL_Window *createWindow(SDL_Window *window,const char *windowName);
SDL_Texture *loadTexture(const char* path, SDL_Renderer *renderTarget);
