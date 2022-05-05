
#include<SDL.h>
#include"..\Header Files\Define.h"
#include"..\Header Files\MathUtil.h"
#include<math.h> 
#include"..\Header Files\Collision.h"
#include"..\Header Files\Process_Event.h"
#include"..\Header Files\WindowSDL.h"
#include<SDL_mixer.h>

//Đếm số lần ghi bàn
void goalCounting(int goalCount1, int goalCount2, SDL_Rect *SgoalCount1, SDL_Rect *SgoalCount2, float FrameW)
{
	SgoalCount1->x = goalCount1 * FrameW;
	SgoalCount2->x = goalCount2 * FrameW;
}

//Định hình vật thể : tọa độ x, y, tọa độ vector vận tốc, lực, góc
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

//Khởi tạo item 
void init_item(itemOpject* item, const char* path, SDL_Renderer* renderTarget)
{
	item->texture = loadTexture(path, renderTarget);
	item->drc.x = 0; item->drc.y = 0; item->drc.h = 0; item->drc.w = 0;
	item->car1_touch = 0;
	item->car2_touch = 0;
	item->touch_time = 0;
}

void goalCheer(SDL_Rect *goalRec, int H, int W, float dental, Opject *alCar1, Opject *alCar2, Opject *alBall, float *delay, int *goalCount1, int *goalCount2, Mix_Chunk *soundEffectGoal)
{
	float rate = 0;//Tốc độ nở ra của hình GOAL
	if (goalRec->h <= H / 1.5)
	{
		rate = 1.5;
	}
	else
		rate = 0;

	goalRec->h += H * rate*dental;
	goalRec->w += W * rate*dental;
	*delay += H * dental / 2.5f;

	if (*delay == H * dental / 2.5f)//Tính thời gian 
		Mix_PlayChannel(-1, soundEffectGoal, 0);

	if (*delay >= H) //Hoãn 1 thời gian sau khi ghi bàn rồi reset bàn chơi
	{
		//Đếm số bàn ghi được của mỗi đội
		if (alBall->x <= 50)
		{
			*goalCount2 += 1;
			if (*goalCount2 == 4)
				*goalCount2 = 0;
		}

		else if (alBall->x >= SCREEN_WIDTH - 90 - BALL_RADIUS / 2 - 10)
		{
			*goalCount1 += 1;
			if (*goalCount1 == 4)
				*goalCount1 = 0;
		}
		//Reset bàn chơi
		initOpject(alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
		initOpject(alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH / 2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
		initOpject(alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);
		goalRec->h = 0;
		goalRec->w = 0;
		//reset thời gian delay
		*delay = 0;
	}
}

//Xử lý (gần như) mọi sự kiện
int processEvents(SDL_Window *window, Opject *alCar1, Opject *alCar2, Opject *alBall, Opject *alNet1, Opject *alNet2, itemOpject* sleep, itemOpject* big)
{
	SDL_Event event;
	int done = 0;//Biến dừng
	const float acc = 3.2f;//Gia tốc của xe
	const float angRate = 2.1f;//Gia tốc góc quay xe

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE://Thoát game
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
			case SDLK_ESCAPE://Thoát game
				done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT://Thoát game

			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (sleep->car2_touch == 0)			// nếu xe 2 dẫm vào item sleep thì xe 1 không được chạy
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

			//Để điều khiển được xe di chuyển theo hướng mà nó đối diện
			//Ta cần tính được sin và cos của góc mà nó đang hướng tới
			//Rồi sau đó nhân với gia tốc và thêm vào thành phần vector vận tốc
			//Vì tọa đồ của màn hình có trục X hướng từ trên xuống dưới nên để -cos
			float cdx = sinf(radiansFromDegrees(alCar1->ang)) * acc * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar1->ang)) * acc * 0.1f;
			alCar1->dx += cdx;
			alCar1->dy += cdy;
		}
		if (state[SDL_SCANCODE_S])
		{
			//Như trên nhưng xe lùi nên += -
			float cdx = sinf(radiansFromDegrees(alCar1->ang)) * 0.1f;
			float cdy = -cosf(radiansFromDegrees(alCar1->ang)) * 0.1f;
			alCar1->dx += -cdx;
			alCar1->dy += -cdy;
		}
	}
	


	if (sleep->car1_touch == 0)
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

	carCollision(alCar1, alCar2, big->car1_touch, big->car2_touch);
	BallCollision(alCar1, alBall, big->car1_touch);
	BallCollision(alCar2, alBall, big->car2_touch);
	NetCollision(alBall, alNet1);
	NetCollision(alBall, alNet2);
	applyForces(alCar1);
	applyForces(alCar2);
	applyForcesBall(alBall, alNet1);


	return done;
}


//Render : Xuất mọi thứ lên trên màn hình
void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall,SDL_Texture *ruler, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball, SDL_Texture *background, SDL_Texture *goal, SDL_Rect goalRect, SDL_Texture *goalCount1, SDL_Texture *goalCount2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, SDL_Rect DgoalCount1, SDL_Rect DgoalCount2, SDL_Texture *goalNet1, SDL_Texture *goalNet2, Opject *alNet1, Opject *alNet2,itemOpject *sleep,itemOpject *Big,itemOpject *magicball)
{
	
	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);


	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background, NULL, NULL);

	SDL_SetRenderDrawColor(renderTarget, 255, 255, 255, 255);

	SDL_Point center1 = { CAR_WIDTH , CAR_HEIGHT + 15 };//Set tâm quay cho xe1
	SDL_Rect rect = { alCar->x, alCar->y, CAR_WIDTH * 2, CAR_HEIGHT * 2 };
	SDL_RenderCopyEx(renderTarget, car1, NULL, &rect, alCar->ang, &center1, 0);

	SDL_Point center2 = { CAR_WIDTH , CAR_HEIGHT + 15 };//Set tâm quay cho xe2
	SDL_Rect rect2 = { alCar2->x, alCar2->y, CAR_WIDTH * 2, CAR_HEIGHT * 2 };
	SDL_RenderCopyEx(renderTarget, car2, NULL, &rect2, alCar2->ang, &center2, 0);

	//Hitbox của quá bóng
	SDL_Rect rectBall = { alBall->x, alBall->y, BALL_RADIUS + 15, BALL_RADIUS + 15 };
	SDL_RenderCopyEx(renderTarget, ball, NULL, &rectBall, alBall->ang, NULL, 0);

	//Đếm bàn thắng
	SDL_RenderCopy(renderTarget, goalCount1, &SgoalCount1, &DgoalCount1);
	SDL_RenderCopy(renderTarget, goalCount2, &SgoalCount2, &DgoalCount2);

	//GOAL
	SDL_RenderCopy(renderTarget, goal, NULL, &goalRect);

	//Render 2 cái lưới, nhưng mà thực sự ko cần thiết lắm
	SDL_Rect rectGoalNet1 = { alNet1->x, alNet1->y, GOAL_WIDTH*2.5, GOAL_HEIGHT*2.5 };
	SDL_Rect rectGoalNet2 = { alNet2->x, alNet2->y, GOAL_WIDTH*2.5, GOAL_HEIGHT*2.5 };
	SDL_RenderCopy(renderTarget, goalNet1, NULL, &rectGoalNet1);
	SDL_RenderCopy(renderTarget, goalNet2, NULL, &rectGoalNet2);


	//Thuowsc ker
	SDL_Rect rulerRect = { SCREEN_WIDTH - GOAL_WIDTH * 2.5, SCREEN_HEIGHT / 2 + GOAL_HEIGHT*1.2, 200, 5 };
	SDL_RenderCopy(renderTarget, ruler, NULL, &rulerRect);

	// Item
	SDL_RenderCopy(renderTarget, sleep->texture, NULL, &sleep->drc);				
	SDL_RenderCopy(renderTarget, Big->texture, NULL, &Big->drc);
	SDL_RenderCopy(renderTarget, magicball->texture, NULL, &magicball->drc);

	//Xuất mọi thứ 
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
	int i = realTime / 15;
	itemRect->x = posX[i];
	itemRect->y = posY[i];
	itemRect->w = 100;
	itemRect->h = 100;
}

// xóa vị trí xuất hiện của item 
void destroy_itemRect(SDL_Rect* itemRect)
{
	itemRect->x = -100;
	itemRect->y = -100;
	itemRect->w = 0;
	itemRect->h = 0;
}

// Khởi tạo, xử lý giữa item với xe 
void item_event(itemOpject* item, Opject* alCar1, Opject* alCar2, int startTime, int step, int effectTime, int realTime, int* item_posX, int* item_posY)
{
	//Khởi tạo item tại 'startTime' và sau 5s tự động xóa item lặp lại sau 'step' 
	if ((realTime >= startTime && realTime <= startTime + 5) || (realTime >= startTime + step && realTime <= startTime + step + 5))
	{
		if (item->car1_touch == 0 && item->car2_touch == 0)
			init_itemRect(&item->drc, item_posX, item_posY, realTime);
	}
	else
	{
		destroy_itemRect(&item->drc);
	}

	//Xác định va trạm giữa xe và các item
	if (collide2d(alCar1->x, alCar1->y, item->drc.x, item->drc.y, CAR_WIDTH * 2, CAR_HEIGHT * 2, 100, 100))
	{
		destroy_itemRect(&item->drc);
		item->car1_touch = 1;
		item->touch_time = realTime;
	}
	if (collide2d(alCar2->x, alCar2->y, item->drc.x, item->drc.y, CAR_WIDTH * 2, CAR_HEIGHT * 2, 100, 100))
	{
		destroy_itemRect(&item->drc);
		item->car2_touch = 1;
		item->touch_time = realTime;
	}

	// Sau 'effectTime' giay xóa bỏ hiệu ứng item 
	if (realTime >= item->touch_time + effectTime)
	{
		item->car1_touch = 0;
		item->car2_touch = 0;
	}

}

//Cần sửa cái item magicball này 
void item_magicball(Opject* alball, itemOpject* magicball, Opject* alCar1, Opject* alCar2, int startTime, int step, int effectTime, int realTime, int* item_posX, int* item_posY)
{
	item_event(magicball, alCar1, alCar2, startTime, step, effectTime, realTime, item_posX, item_posY);
	if (magicball->car1_touch == 1)
	{
		initOpject(alball, 1400, 440, 0);
		magicball->car1_touch == 0;
	}
	else if (magicball->car2_touch == 1)
	{
		initOpject(alball, 200, 440, 0);
		magicball->car2_touch == 0;
	}
}
