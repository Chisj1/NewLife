
#include "SDL.h"
#include "SDL_image.h"
#include "Header Files\MathUtil.h"
#include <math.h>
#include <stdio.h>
#include"Header Files\Define.h"
#include"Header Files\WindowSDL.h"
#include"Header Files\Collision.h"
#include"Header Files\Process_Event.h"

int main(int argc, char *argv[])
{
	static SDL_Texture *background = NULL;
	static SDL_Texture *car1 = NULL;
	static SDL_Texture *car2 = NULL;
	static SDL_Texture *ball = NULL;
	static SDL_Texture *goal = NULL;
	SDL_Window *window = NULL;
	window = SDL_CreateWindow("Rocket League 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		printf("Init Video False ! \n %s", SDL_GetError());
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	Opject alCar1, alCar2, alBall;

	initOpject(&alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
	initOpject(&alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH/2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
	initOpject(&alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);


	background = loadTexture("bg.png", renderTarget);
	car1 = loadTexture("car1.png", renderTarget);
	car2 = loadTexture("car2.png", renderTarget);
	ball = loadTexture("ball.png", renderTarget);
	goal = loadTexture("goal.png", renderTarget);
	SDL_Rect goalRect;
	SDL_QueryTexture(goal, NULL, NULL, &goalRect.w, &goalRect.h);
	const int W = goalRect.w;
	const int H = goalRect.h;
	goalRect.w = 0;
	goalRect.h = 0;
	goalRect.x = 800-W/3;
	goalRect.y = 450-H/3;

	int done = 0;

	//Event loop
	float currTime = 0;
	float preTime = 0;
	float delta = 0;
	float delay = 0;
	while (!done)
	{

		//Check for events
		preTime = currTime;
		currTime = SDL_GetTicks();
		delta = (currTime - preTime) / 1000.0f;

		done = processEvents(window, &alCar1, &alCar2, &alBall);
		if (isGoal(alBall))
		{
			goalCheer(&goalRect, H, W, delta, &alCar1, &alCar2, &alBall, &delay);
		}

		doRender(renderTarget, &alCar1, &alCar2, &alBall, car1, car2, ball, background, goal, goalRect);
		SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderTarget);
	renderTarget = NULL;
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(car2);
	SDL_DestroyTexture(car1);
	SDL_DestroyTexture(ball);
	car1 = NULL;
	car2 = NULL;
	background = NULL;
	ball = NULL;
	IMG_Quit();
	SDL_Quit();
	return 0;
}
