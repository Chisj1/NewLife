
#include "SDL.h"
#include "SDL_image.h"
#include "..\Header Files\MathUtil.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"..\Header Files\Define.h"
#include"..\Header Files\WindowSDL.h"
#include"..\Header Files\Collision.h"
#include"..\Header Files\Process_Event.h"
#include "SDL_mixer.h"

//TODO thay nền menu + làm kết game


int main(int argc, char *argv[])
{
	//Định hình các texture
	srand((int)time(NULL));
	static SDL_Texture *background = NULL;
	static SDL_Texture *car1 = NULL;
	static SDL_Texture *car2 = NULL;
	static SDL_Texture *ball = NULL;
	static SDL_Texture *goal = NULL;
	static SDL_Texture *goalCountTex1 = NULL;
	static SDL_Texture *goalCountTex2 = NULL;
	static SDL_Texture *goalNet1 = NULL;
	static SDL_Texture *goalNet2 = NULL;
	static SDL_Texture *ruler = NULL;
	static SDL_Texture* background_menu = NULL;
	static SDL_Texture* start = NULL;
	static SDL_Texture* start1 = NULL;
	static SDL_Texture* exit1 = NULL;
	static SDL_Texture* exit2 = NULL;
	static SDL_Texture* playagain1 = NULL;
	static SDL_Texture* playagain2 = NULL;
	static SDL_Texture* win = NULL;
	static SDL_Texture* lost = NULL;
	static SDL_Texture* cup = NULL;
	//Khởi tạo màn hình chính
	SDL_Window *window = NULL;
	window = SDL_CreateWindow("Rocket League 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	//Khởi tạo render
	SDL_Renderer *renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Khởi tạo SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		printf("Init Video False ! \n %s", SDL_GetError());
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		printf("Error \n%s", Mix_GetError());

	//Khởi tạo Opject
	Opject alCar1, alCar2, alBall, alNet1, alNet2;

	initOpject(&alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
	initOpject(&alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH / 2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
	initOpject(&alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);
	initOpject(&alNet1, 0, SCREEN_HEIGHT / 2 - GOAL_HEIGHT * 1.27, 0);
	initOpject(&alNet2, SCREEN_WIDTH - GOAL_WIDTH * 2.5, SCREEN_HEIGHT / 2 - GOAL_HEIGHT * 1.27, 0);


	//Tải hình ảnh cho texture
	background = loadTexture(".\\Resource Files\\bgOri.png", renderTarget);
	car1 = loadTexture(".\\Resource Files\\car1.png", renderTarget);
	car2 = loadTexture(".\\Resource Files\\car2.png", renderTarget);
	ball = loadTexture(".\\Resource Files\\ball.png", renderTarget);
	goal = loadTexture(".\\Resource Files\\goal.png", renderTarget);
	goalNet1 = loadTexture(".\\Resource Files\\goalNet.png", renderTarget);
	goalNet2 = loadTexture(".\\Resource Files\\goalNet.png", renderTarget);

	SDL_Rect rulerRect;

	ruler = loadTexture("Ruler.png", renderTarget);
	if (ruler == NULL)
		printf("Error Ruler");


	//Hình ảnh GOAL ăn mừng
	SDL_Rect goalRect;
	SDL_QueryTexture(goal, NULL, NULL, &goalRect.w, &goalRect.h);
	const int W = goalRect.w;
	const int H = goalRect.h;
	goalRect.w = 0;
	goalRect.h = 0;
	goalRect.x = 800 - W / 3;
	goalRect.y = 450 - H / 3;

	//Hình ảnh đếm số bàn thắng
	goalCountTex1 = loadTexture(".\\Resource Files\\Goal\\goalCount3.png", renderTarget);
	goalCountTex2 = loadTexture(".\\Resource Files\\Goal\\goalCount3.png", renderTarget);

	SDL_Rect SgoalCount1 = { 0, 0, 40 , 52 };
	SDL_Rect SgoalCount2 = { 0, 0, 40 , 52 };
	SDL_Rect DgoalCount1 = { 490, 20, 184, 240 };
	SDL_Rect DgoalCount2 = { 900, 20, 184, 240 };

	//Khởi tạo item ,tạo mảng random vitri item 
	itemOpject sleep, Big, magicball;
	init_item(&sleep, ".\\Resource Files\\item.png", renderTarget);
	init_item(&Big, ".\\Resource Files\\item.png", renderTarget);
	init_item(&magicball, ".\\Resource Files\\item.png", renderTarget);
	int item_posX[20], item_posY[20];
	random_pos(item_posX, 350, SCREEN_WIDTH - 350);
	random_pos(item_posY, 100, SCREEN_HEIGHT - 100);

	//Biến dừng
	int done = 0;

	//Biến đếm thời gian
	float currTime = 0;
	float preTime = 0;
	float delta = 0;
	float delay = 0;
	int realTime = 0;
	int timestart = 0;
	int goalCount1 = 0;
	int goalCount2 = 0;

	//Âm thanh nền và hiệu ứng
	Mix_Music *bgm = Mix_LoadMUS(".\\Resource Files\\Music\\gameBGmusic.mp3");
	if (bgm == NULL)
		printf("Music Er");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	Mix_Chunk *soundEffectGoal = Mix_LoadWAV(".\\Resource Files\\Music\\goal.wav");
	Mix_Chunk *soundEngGame = Mix_LoadWAV(".\\Resource Files\\Music\\EngameSound.wav");
	if (soundEffectGoal == NULL)
		printf("Eror SOund");

	//Biến điều kiện chạy hàm item
	int a = 1, b = 1;

	//Hiển thị menu lúc vào game
	int Menu_check = menu(renderTarget, window, background_menu, start, exit1, start1, exit2);
	if (Menu_check)
	{
		exit(0);
	}
	else
	{
		//Bắt đầu game
		timestart = SDL_GetTicks();
		while (!done)
		{

			preTime = currTime;
			currTime = (float)SDL_GetTicks();
			delta = (currTime - preTime) / 1000.0f;
			realTime = (int)((currTime - (float)timestart) / 1000);

			//Chơi nhạc BG
			if (!Mix_PlayingMusic())
				Mix_PlayMusic(bgm, -1);
			done = processEvents(window, &alCar1, &alCar2, &alBall, &alNet1, &alNet2, &sleep, &Big);

			//Ăn mừng bàn thắng 
			if (isGoal(&alBall))
			{
				goalCheer(&goalRect, H, W, delta, &alCar1, &alCar2, &alBall, &delay, &goalCount1, &goalCount2, soundEffectGoal);
			}

			//Tính điểm bàn thắng	
			goalCounting(goalCount1, goalCount2, &SgoalCount1, &SgoalCount2, 40);

			//Item hiệu ứng, item chỉ xuất hiện 2 lần mỗi loại
			item_event(&Big, &alCar1, &alCar2, 30, 75, 7, realTime, item_posX, item_posY, &b);
			item_event(&sleep, &alCar1, &alCar2, 60, 150, 6, realTime, item_posX, item_posY, &b);
			item_magicball(&alBall, &magicball, &alCar1, &alCar2, 105, 135, 5, realTime, item_posX, item_posY, &a);

			// endgame or playagain
			if (realTime >= 15) {
				done = endgame(renderTarget, window, background, win, lost, playagain1, exit1, playagain2, exit2, car1, car2, goalCountTex1, goalCountTex2, SgoalCount1, SgoalCount2, &goalCount1, &goalCount2, &alCar1, &alCar2, &alBall, cup, soundEngGame);
				timestart = SDL_GetTicks();
			}
			//Render tất cả mọi thứ
			doRender(renderTarget, &alCar1, &alCar2, &alBall, ruler, car1, car2, ball, background, goal, goalRect, goalCountTex1, goalCountTex2, SgoalCount1, SgoalCount2, DgoalCount1, DgoalCount2, goalNet1, goalNet2, &alNet1, &alNet2, &sleep, &Big, &magicball);

			SDL_Delay(10);
		}
	}


	//Dọn dẹp mọi thứ
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
	SDL_DestroyTexture(start);
	SDL_DestroyTexture(start1);
	SDL_DestroyTexture(exit1);
	SDL_DestroyTexture(exit2);
	SDL_DestroyTexture(background_menu);
	SDL_DestroyTexture(cup);
	start = NULL;
	start1 = NULL;
	exit1 = NULL;
	exit2 = NULL;
	background_menu = NULL;
	cup = NULL;
	goalCountTex1 = NULL;
	goalCountTex2 = NULL;
	car1 = NULL;
	car2 = NULL;
	background = NULL;
	ball = NULL;
	goalNet1 = NULL;
	goalNet2 = NULL;
	IMG_Quit();
	SDL_Quit();
	return 0;
}
