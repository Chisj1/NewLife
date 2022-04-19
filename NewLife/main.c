
#include "SDL.h"
#include "SDL_image.h"
#include "MathUtil.h"
#include <math.h>
#include <stdio.h>
#include"Define.h"
#include"WindowSDL.h"
#include"Collision.h"
#include"Process_Event.h"

int main(int argc, char *argv[])
{
	static SDL_Texture *background = NULL;
	static SDL_Texture *car1 = NULL;
	static SDL_Texture *car2 = NULL;
	static SDL_Texture *ball = NULL;
	SDL_Window *window = NULL;
	window = SDL_CreateWindow("Rocket League 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		printf("Init Video False ! \n %s", SDL_GetError());

	Opject alCar1, alCar2, alBall;

	initOpject(&alCar1, 220, 140);
	initOpject(&alCar2, 520, 360);
	initOpject(&alBall, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	

	background = loadTexture("bg2.jpg", renderTarget);
	car1 = loadTexture("car1.png", renderTarget);
	car2 = loadTexture("car2.png", renderTarget);
	ball = loadTexture("ball.png", renderTarget);

	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;

	//Event loop
	while (!done)
	{
		//Check for events
		done = processEvents(window, &alCar1, &alCar2, &alBall);

		//Render display
		doRender(renderTarget, &alCar1, &alCar2, &alBall, car1, car2, ball, background);

		//don't burn up the CPU
		SDL_Delay(10);
	}


	// Close and destroy the window
	// Clean up
	SDL_Quit();
	return 0;
}
