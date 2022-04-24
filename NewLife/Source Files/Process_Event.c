
#include<SDL.h>
#include"..\Header Files\Define.h"
#include"..\Header Files\MathUtil.h"
#include<math.h> 
#include"..\Header Files\Collision.h"
#include"..\Header Files\Process_Event.h"
#include"..\Header Files\WindowSDL.h"
#include<SDL_mixer.h>

void goalCounting(int goalCount1, int goalCount2, SDL_Rect *SgoalCount1, SDL_Rect *SgoalCount2, float FrameW)
{
	SgoalCount1->x = goalCount1 * FrameW;
	SgoalCount2->x = goalCount2 * FrameW;
}

void initOpject(Opject *opjects, int posX, int posY, int angOject)
{
	opjects->x = posX;
	opjects->y = posY;
	opjects->dx = 0;
	opjects->dy = 0;
	opjects->gdx = 0;
	opjects->gdy = 0;
	opjects->ang = angOject;
}
void goalCheer(SDL_Rect *goalRec, int H, int W, float dental, Opject *alCar1, Opject *alCar2, Opject *alBall, float *delay, int *goalCount1, int *goalCount2, Mix_Chunk *soundEffectGoal)
{
	float rate = 0;
	if (goalRec->h <= H / 1.5)
	{
		rate = 1.5;
	}
	else
		rate = 0;

	goalRec->h += H * rate*dental;
	goalRec->w += W * rate*dental;
	*delay += H * dental / 2.5f;

	if (*delay == H * dental / 2.5f)
		Mix_PlayChannel(-1, soundEffectGoal, 0);

	if (*delay >= H)
	{
		if (alBall->x <= 25)
		{
			*goalCount2 += 1;
			if (*goalCount2 == 4)
				*goalCount2 = 0;
		}

		else if (alBall->x >= 1500)
		{
			*goalCount1 += 1;
			if (*goalCount1 == 4)
				*goalCount1 = 0;
		}
		initOpject(alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
		initOpject(alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH / 2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
		initOpject(alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);
		goalRec->h = 0;
		goalRec->w = 0;
		*delay = 0;
	}
}

int processEvents(SDL_Window *window, Opject *alCar1, Opject *alCar2, Opject *alBall, Opject *alNet1, Opject *alNet2, itemOpject *item)
{
	SDL_Event event;
	int done = 0;
	const float acc = 2.0f*1.6f;
	const float angRate = 2.1f;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT:

			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	if (item->car2_touch == 0)						// nếu xe 2 dẫm vào item sleep thì xe 1 k được chạy 
	{
		if (state[SDL_SCANCODE_A])
		{
			alCar1->ang -= angRate;
		}
		if (state[SDL_SCANCODE_D])
		{
			alCar1->ang += angRate;
		}
		if (state[SDL_SCANCODE_W])
		{
			float cdx = sinf(radiansFromDegrees(alCar1->ang)) * acc * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar1->ang)) * acc * 0.1f;

			alCar1->dx += cdx;
			alCar1->dy += cdy;
		}
		if (state[SDL_SCANCODE_S])
		{
			float cdx = sinf(radiansFromDegrees(alCar1->ang)) * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar1->ang)) * 0.1f;
			alCar1->dx += -cdx;
			alCar1->dy += -cdy;
		}
	}
	if (item->car1_touch == 0)						// nếu xe 1 dẫm vào item sleep thì xe 2 k được chạy 
	{
		if (state[SDL_SCANCODE_LEFT])
		{
			alCar2->ang -= angRate;
		}
		if (state[SDL_SCANCODE_RIGHT])
		{
			alCar2->ang += angRate;
		}
		if (state[SDL_SCANCODE_UP])
		{
			float cdx = sinf(radiansFromDegrees(alCar2->ang)) * acc * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar2->ang)) * acc * 0.1f;

			alCar2->dx += cdx;
			alCar2->dy += cdy;
		}
		if (state[SDL_SCANCODE_DOWN])
		{
			float cdx = sinf(radiansFromDegrees(alCar2->ang)) * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar2->ang)) * 0.1f;

			alCar2->dx += -cdx;
			alCar2->dy += -cdy;
		}
	}


	carCollision(alCar1, alCar2);
	BallCollision(alCar1, alBall);
	BallCollision(alCar2, alBall);
	applyForces(alCar1);
	applyForces(alCar2);
	applyForcesBall(alBall);


	return done;
}

void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball, SDL_Texture *background, SDL_Texture *goal, SDL_Rect goalRect, SDL_Texture *goalCount1, SDL_Texture *goalCount2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, SDL_Rect DgoalCount1, SDL_Rect DgoalCount2, SDL_Texture *goalNet1, SDL_Texture *goalNet2, Opject *alNet1, Opject *alNet2, itemOpject *item)
{

	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);

	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background, NULL, NULL);

	SDL_SetRenderDrawColor(renderTarget, 255, 255, 255, 255);

	SDL_Rect rect = { alCar->x, alCar->y, CAR_WIDTH * 2, CAR_HEIGHT * 2 };
	SDL_RenderCopyEx(renderTarget, car1, NULL, &rect, alCar->ang, NULL, 0);

	SDL_Rect rect2 = { alCar2->x, alCar2->y, CAR_WIDTH * 2, CAR_HEIGHT * 2 };
	SDL_RenderCopyEx(renderTarget, car2, NULL, &rect2, alCar2->ang, NULL, 0);

	SDL_Rect rectBall = { alBall->x, alBall->y, BALL_RADIUS + 15, BALL_RADIUS + 15 };
	SDL_RenderCopyEx(renderTarget, ball, NULL, &rectBall, alBall->ang, NULL, 0);

	SDL_RenderCopy(renderTarget, goalCount1, &SgoalCount1, &DgoalCount1);
	SDL_RenderCopy(renderTarget, goalCount2, &SgoalCount2, &DgoalCount2);
	SDL_RenderCopy(renderTarget, goal, NULL, &goalRect);

	SDL_Rect rectGoalNet1 = { alNet1->x, alNet1->y, 50*2, 120*2 };
	SDL_Rect rectGoalNet2 = { alNet2->x, alNet2->y, 50*2, 120*2 };
	SDL_RenderCopy(renderTarget, goalNet1, NULL, &rectGoalNet1);
	SDL_RenderCopy(renderTarget, goalNet2, NULL, &rectGoalNet2);
	
	SDL_RenderCopy(renderTarget, item->texture, &item->src, &item->drc);				// xuất ra màn hình item 
	
	SDL_RenderPresent(renderTarget);
}

// tạo mảng random vị trí 
void random_pos(int* pos, int left, int right)
{
	for (int i = 0; i < sizeof(pos); i++) {
		pos[i] = left + rand() % (right - left);
	}
}

// khởi tạo random  vị trí item 
void init_itemRect(SDL_Rect* itemRect, int* posX, int* posY, int realTime)
{
	static int i = 0;
	i = (realTime / 10)  ;
	itemRect->x = posX[i];
	itemRect->y = posY[i];
	itemRect->w = 100;
	itemRect->h = 100;
}

// xóa vị trí xuất hiện của item 
void destroy_itemRect(SDL_Rect* itemRect) 
{
	itemRect->x = 0;
	itemRect->y = 0;
	itemRect->w = 0;
	itemRect->h = 0;
}

void item_event(itemOpject *item,Opject *alCar1,Opject *alCar2,int startTime,int endTime, int realTime, int *item_posX, int *item_posY)
{
	// sau mỗi 30 s khởi tạo item tại startTime và sau endTime tự động xóa item 
	if ((realTime / 10) % 3 == 0 && realTime % 30 >= startTime && realTime % 30 <= endTime && realTime / 10 > 0) {
		if (item->car1_touch == 0 && item->car2_touch == 0)
			init_itemRect(&item->drc, item_posX, item_posY, realTime);
	}
	else {
		destroy_itemRect(&item->drc);
	}

	// nếu item được khởi tạo thì xét vị trí của các xe 
	if (item->drc.x != 0 && item->drc.y != 0)
	{
		if (alCar1->x >= item->drc.x && alCar1->x <= item->drc.x + 100 && alCar1->y >= item->drc.y && alCar1->y <= item->drc.y + 100) {
			destroy_itemRect(&item->drc);
			item->car1_touch = 1;
			item->touch_time = realTime;
		}
		if (alCar2->x >= item->drc.x && alCar2->x <= item->drc.x + 100 && alCar2->y >= item->drc.y && alCar2->y <= item->drc.y + 100) {
			destroy_itemRect(&item->drc);
			item->car2_touch = 1;
			item->touch_time = realTime;
		}
	}
	
	// sau 5s xóa bỏ hiệu ứng item 
	if (realTime >= item->touch_time + 5) {
		item->car1_touch = 0;
		item->car2_touch = 0;
	}
}
