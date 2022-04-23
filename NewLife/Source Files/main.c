
#include "SDL.h"
#include "SDL_image.h"
#include "..\Header Files\MathUtil.h"
#include <math.h>
#include <stdio.h>
#include"..\Header Files\Define.h"
#include"..\Header Files\WindowSDL.h"
#include"..\Header Files\Collision.h"
#include"..\Header Files\Process_Event.h"
#include "SDL_mixer.h"


int main(int argc, char *argv[])
{
	static SDL_Texture *background = NULL;
	static SDL_Texture *car1 = NULL;
	static SDL_Texture *car2 = NULL;
	static SDL_Texture *ball = NULL;
	static SDL_Texture *goal = NULL;
	static SDL_Texture *goalCountTex1 = NULL;
	static SDL_Texture *goalCountTex2 = NULL;
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
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		printf("Error \n%s", Mix_GetError());


	Opject alCar1, alCar2, alBall;

	initOpject(&alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
	initOpject(&alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH / 2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
	initOpject(&alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);

	background = loadTexture(".\\Resource Files\\bg.png", renderTarget);
	car1 = loadTexture(".\\Resource Files\\car1.png", renderTarget);
	car2 = loadTexture(".\\Resource Files\\car2.png", renderTarget);
	ball = loadTexture(".\\Resource Files\\ball.png", renderTarget);
	goal = loadTexture(".\\Resource Files\\goal.png", renderTarget);
	SDL_Rect goalRect;
	SDL_QueryTexture(goal, NULL, NULL, &goalRect.w, &goalRect.h);
	const int W = goalRect.w;
	const int H = goalRect.h;
	goalRect.w = 0;
	goalRect.h = 0;
	goalRect.x = 800 - W / 3;
	goalRect.y = 450 - H / 3;

	goalCountTex1 = loadTexture(".\\Resource Files\\Goal\\goalCount.png", renderTarget);
	goalCountTex2 = loadTexture(".\\Resource Files\\Goal\\goalCount.png", renderTarget);

	SDL_Rect SgoalCount1 = { 0, 0, 48 , 48 };
	SDL_Rect SgoalCount2 = { 0, 0, 48 , 48 };
	SDL_Rect DgoalCount1 = { 450, 20, 240, 240 };
	SDL_Rect DgoalCount2 = { 900, 20, 240, 240 };
	int stop = 0;

	int done = 0;

	float currTime = 0;
	float preTime = 0;
	float delta = 0;
	float delay = 0;

	int goalCount1 = 0;
	int goalCount2 = 0;

	Mix_Music *bgm = Mix_LoadMUS(".\\Resource Files\\Music\\gameBGmusic.mp3");
	if (bgm == NULL)
		printf("Music Er");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
	Mix_Chunk *soundEffectGoal = Mix_LoadWAV(".\\Resource Files\\Music\\goal.wav");
	if (soundEffectGoal == NULL)
		printf("Eror SOund");
	while (!done)
	{

		preTime = currTime;
		currTime = (float) SDL_GetTicks();
		delta = (currTime - preTime) / 1000.0f;
		if (!Mix_PlayingMusic())
			Mix_PlayMusic(bgm, -1);
		done = processEvents(window, &alCar1, &alCar2, &alBall);

		if (isGoal(&alBall))
		{
			goalCheer(&goalRect, H, W, delta, &alCar1, &alCar2, &alBall, &delay, &goalCount1, &goalCount2, soundEffectGoal);
		}
			
		goalCounting(goalCount1, goalCount2, &SgoalCount1, &SgoalCount2, 48);

		doRender(renderTarget, &alCar1, &alCar2, &alBall, car1, car2, ball, background, goal, goalRect, goalCountTex1, goalCountTex2, SgoalCount1, SgoalCount2, DgoalCount1, DgoalCount2);
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
	SDL_DestroyTexture(goalCountTex1);
	SDL_DestroyTexture(goalCountTex2);
	goalCountTex1 = NULL;
	goalCountTex2 = NULL;
	car1 = NULL;
	car2 = NULL;
	background = NULL;
	ball = NULL;
	IMG_Quit();
	SDL_Quit();
	return 0;
}
