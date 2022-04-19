#include"Header Files\WindowSDL.h"
#include"Header Files\Define.h"
#include<SDL.h>
#include <SDL_image.h>


SDL_Texture * loadTexture(const char * path, SDL_Renderer * renderTarget)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL)
		printf("Error load surface\n");
	else
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
	SDL_FreeSurface(surface);
	return texture;
}
