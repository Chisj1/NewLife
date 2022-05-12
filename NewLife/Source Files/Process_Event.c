
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
			if (*goalCount2 > 9)
				*goalCount2 = 0;
		}

		else if (alBall->x >= SCREEN_WIDTH - 90 - BALL_RADIUS / 2 - 10)
		{
			*goalCount1 += 1;
			if (*goalCount1 > 9)
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
			float cdx = sinf(radiansFromDegrees(alCar1->ang)) * acc * 0.1f * ((float)big->car1_touch *0.5f + 1);
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
			float cdx = sinf(radiansFromDegrees(alCar2->ang)) * acc * 0.1f * ((float)big->car1_touch *0.5f + 1);
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
void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall, SDL_Texture *ruler, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball, SDL_Texture *background, SDL_Texture *goal, SDL_Rect goalRect, SDL_Texture *goalCount1, SDL_Texture *goalCount2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, SDL_Rect DgoalCount1, SDL_Rect DgoalCount2, SDL_Texture *goalNet1, SDL_Texture *goalNet2, Opject *alNet1, Opject *alNet2, itemOpject *sleep, itemOpject *Big, itemOpject *magicball)
{

	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);


	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background, NULL, NULL);

	SDL_SetRenderDrawColor(renderTarget, 255, 255, 255, 255);

	//Đếm bàn thắng
	SDL_RenderCopy(renderTarget, goalCount1, &SgoalCount1, &DgoalCount1);
	SDL_RenderCopy(renderTarget, goalCount2, &SgoalCount2, &DgoalCount2);
	
	//item
	SDL_RenderCopy(renderTarget, sleep->texture, NULL, &sleep->drc);
	SDL_RenderCopy(renderTarget, Big->texture, NULL, &Big->drc);
	SDL_RenderCopy(renderTarget, magicball->texture, NULL, &magicball->drc);
	//GOAL
	SDL_RenderCopy(renderTarget, goal, NULL, &goalRect);


	SDL_Point center1 = { CAR_WIDTH * (Big->car1_touch + 1) , (CAR_HEIGHT + 15) * (Big->car1_touch + 1) };
	SDL_Point center2 = { CAR_WIDTH * (Big->car2_touch + 1) , (CAR_HEIGHT + 15) * (Big->car2_touch + 1) };//Set tâm quay cho 2 xe
	SDL_Rect rect = { alCar->x, alCar->y, CAR_WIDTH * 2 * (Big->car1_touch + 1), CAR_HEIGHT * 2 * (Big->car1_touch + 1) };
	SDL_RenderCopyEx(renderTarget, car1, NULL, &rect, alCar->ang, &center1, 0);
	SDL_Rect rect2 = { alCar2->x, alCar2->y, CAR_WIDTH * 2 * (Big->car2_touch + 1), CAR_HEIGHT * 2 * (Big->car2_touch + 1) };
	SDL_RenderCopyEx(renderTarget, car2, NULL, &rect2, alCar2->ang, &center2, 0);

	//Hitbox của quá bóng
	SDL_Rect rectBall = { alBall->x, alBall->y, BALL_RADIUS + 15, BALL_RADIUS + 15 };
	SDL_RenderCopyEx(renderTarget, ball, NULL, &rectBall, alBall->ang, NULL, 0);

	

	//Render 2 cái lưới
	SDL_Rect rectGoalNet1 = { alNet1->x, alNet1->y, GOAL_WIDTH*2.5, GOAL_HEIGHT*2.5 };
	SDL_Rect rectGoalNet2 = { alNet2->x, alNet2->y, GOAL_WIDTH*2.5, GOAL_HEIGHT*2.5 };
	SDL_RenderCopy(renderTarget, goalNet1, NULL, &rectGoalNet1);
	SDL_RenderCopy(renderTarget, goalNet2, NULL, &rectGoalNet2);


	//Thuowsc ker
	//SDL_Rect rulerRect = { SCREEN_WIDTH - GOAL_WIDTH * 2.5, SCREEN_HEIGHT / 2 + GOAL_HEIGHT * 1.2, 200, 5 };
	//SDL_RenderCopy(renderTarget, ruler, NULL, &rulerRect);

	// Item


	
	//Xuất mọi thứ 
	SDL_RenderPresent(renderTarget);
}

// tạo mảng random vị trí 
void random_pos(int* pos, int left, int right)
{
	for (int i = 0; i < 20; i++) {
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
void item_event(itemOpject* item, Opject* alCar1, Opject* alCar2, int startTime, int againTime, int effectTime, int realTime, int* item_posX, int* item_posY, int* a)
{
	//Khởi tạo item tại 'startTime' và sau 5s tự động xóa item lặp lại tại 'againTime' 
	if ((realTime >= startTime && realTime <= startTime + 5) || (realTime >= againTime && realTime <= againTime + 5))
	{
		if (*a == 1)
		{
			if (item->car1_touch == 0 && item->car2_touch == 0)
			{
				init_itemRect(&item->drc, item_posX, item_posY, realTime);
				//printf("%d  ", realTime);
			}
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
	}
	else
	{
		destroy_itemRect(&item->drc);
	}

	// Sau 'effectTime' giay xóa bỏ hiệu ứng item 
	if (realTime >= item->touch_time + effectTime)
	{
		item->car1_touch = 0;
		item->car2_touch = 0;
		*a = 1;
	}

}

//Sửa xong r nhá, hơi lằng nhằng nhưng chạy được :))
void item_magicball(Opject* alball, itemOpject* magicball, Opject* alCar1, Opject* alCar2, int startTime, int againTime, int effectTime, int realTime, int* item_posX, int* item_posY, int* a)
{
	item_event(magicball, alCar1, alCar2, startTime, againTime, effectTime, realTime, item_posX, item_posY, a);
	if (*a == 1)
	{
		if (magicball->car1_touch == 1)
		{
			initOpject(alball, 1250, 440, 0);
			*a = 0;
		}
		else if (magicball->car2_touch == 1)
		{
			initOpject(alball, 350, 440, 0);
			*a = 0;
		}
	}
}

int CheckForcusWithRect(int x, int y, SDL_Rect rect)
{
	if (x >= rect.x && x <= rect.x + rect.w &&
		y >= rect.y && y <= rect.y + rect.h)
	{
		return 1;
	}
	return 0;
}
int menu(SDL_Renderer* renderTarget, SDL_Window* window, SDL_Texture* background_menu, SDL_Texture* start, SDL_Texture* exit1, SDL_Texture* start1, SDL_Texture* exit2)
{

	background_menu = loadTexture(".\\Resource Files\\Menu\\test.png", renderTarget);
	start = loadTexture(".\\Resource Files\\Menu\\start.png", renderTarget);
	exit1 = loadTexture(".\\Resource Files\\Menu\\exit1.png", renderTarget);
	start1 = loadTexture(".\\Resource Files\\Menu\\start1.png", renderTarget);
	exit2 = loadTexture(".\\Resource Files\\Menu\\exit2.png", renderTarget);
	SDL_Rect drc_start = { 450,300,674,124 };
	SDL_Rect drc_exit1 = { 450,500,674,124 };
	SDL_Rect drc_start1 = { 450,300,674,124 };
	SDL_Rect drc_exit2 = { 450,500,674,124 };
	int done;
	SDL_Event  m_event;
	int xm = 0;
	int ym = 0;

	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);

	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background_menu, NULL, NULL);
	SDL_RenderCopy(renderTarget, start, NULL, &drc_start);
	SDL_RenderCopy(renderTarget, exit1, NULL, &drc_exit1);
	SDL_RenderPresent(renderTarget);

	while (1)
	{
		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
			{
				return 1;
			}
			break;
			case SDL_MOUSEMOTION:
			{
				xm = m_event.motion.x;
				ym = m_event.motion.y;


				if (CheckForcusWithRect(xm, ym, drc_start))
				{
					SDL_RenderCopy(renderTarget, start1, NULL, &drc_start1);
					SDL_RenderPresent(renderTarget);
				}
				else if (CheckForcusWithRect(xm, ym, drc_exit1))
				{
					SDL_RenderCopy(renderTarget, exit2, NULL, &drc_exit2);
					SDL_RenderPresent(renderTarget);
				}
				else
				{
					SDL_RenderCopy(renderTarget, start, NULL, &drc_start);
					SDL_RenderCopy(renderTarget, exit1, NULL, &drc_exit1);
					SDL_RenderPresent(renderTarget);
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				xm = m_event.button.x;
				ym = m_event.button.y;
				if (CheckForcusWithRect(xm, ym, drc_start))
				{
					return 0;
				}
				if (CheckForcusWithRect(xm, ym, drc_exit1))
				{
					return 1;
				}
			}
			break;
			}
		}
	}
}

// TODO cần làm ảnh chữ "playagain" , "Win", "Lost" rồi chỉnh rect của các ảnh 
int endgame(SDL_Renderer* renderTarget, SDL_Window* window, SDL_Texture* background, SDL_Texture* win, SDL_Texture* lost, SDL_Texture* playagain1, SDL_Texture* exit1, SDL_Texture* playagain2, SDL_Texture* exit2, SDL_Texture* car1, SDL_Texture* car2, SDL_Texture* goalCountTex1, SDL_Texture* goalCountTex2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, int* goalCount1, int* goalCount2, Opject* alCar1, Opject* alCar2, Opject* alBall,SDL_Texture* cup, Mix_Chunk *endGameSound)
{

	playagain1 = loadTexture(".\\Resource Files\\EndGame\\Replay1.png", renderTarget);
	playagain2 = loadTexture(".\\Resource Files\\EndGame\\Replay2.png", renderTarget);
	exit1 = loadTexture(".\\Resource Files\\EndGame\\Exit1.png", renderTarget);
	exit2 = loadTexture(".\\Resource Files\\EndGame\\Exit2.png", renderTarget);
	//win = loadTexture(".\\Resource Files\\Menu\\win.png", renderTarget);
	//lost = loadTexture(".\\Resource Files\\Menu\\lost.png", renderTarget);
	cup = loadTexture(".\\Resource Files\\EndGame\\Cup.png", renderTarget);
	SDL_Rect drc_playagain = { SCREEN_WIDTH / 2 - 160, 600, 337, 62 };
	SDL_Rect drc_exit = { SCREEN_WIDTH / 2 - 160,680, 337, 62 };
	SDL_Rect drc_goalcount1 = { 450, SCREEN_HEIGHT / 2 - 75, 150, 150 };
	SDL_Rect drc_goalcount2 = { 1000, SCREEN_HEIGHT / 2 - 75, 150, 150 };
	SDL_Rect drc_car1 = { 200, SCREEN_HEIGHT / 2 - 100, 120 , 200 };
	SDL_Rect drc_car2 = { 1280, SCREEN_HEIGHT / 2 - 100, 120 , 200 };
	SDL_Rect drc_cup = { SCREEN_WIDTH / 2-235,SCREEN_HEIGHT / 2-350 ,500,500 };
	Mix_PlayChannel(-1, endGameSound, 0);
	/*if (*goalCount1 > *goalCount2)
	{
		SDL_Rect drc_win =  {};
		SDL_Rect drc_lost = {};
	}
	else if (*goalCount1 < *goalCount2)
	{
		SDL_Rect drc_win = { };
		SDL_Rect drc_lost = {};
	}
	else if (*goalCount1 == *goalCount2)
	{
		lost = win;
		SDL_Rect drc_win = { };
		SDL_Rect drc_lost = {};
	}
	*/

	SDL_Event m_event;
	int xm = 0;
	int ym = 0;

	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);
	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background, NULL, NULL);
	SDL_RenderCopy(renderTarget, car1, NULL, &drc_car1);
	SDL_RenderCopy(renderTarget, car2, NULL, &drc_car2);
	SDL_RenderCopy(renderTarget, goalCountTex1, &SgoalCount1, &drc_goalcount1);
	SDL_RenderCopy(renderTarget, goalCountTex1, &SgoalCount2, &drc_goalcount2);
	SDL_RenderCopy(renderTarget, playagain1, NULL, &drc_playagain);
	SDL_RenderCopy(renderTarget, exit1, NULL, &drc_exit);
	SDL_RenderCopy(renderTarget, cup, NULL, &drc_cup);
	SDL_RenderPresent(renderTarget);

	while (1)
	{
		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
			{
				return 1;
			}
			break;
			case SDL_MOUSEMOTION:
			{
				xm = m_event.motion.x;
				ym = m_event.motion.y;

				if (CheckForcusWithRect(xm, ym, drc_playagain))
				{
					SDL_RenderCopy(renderTarget, playagain2, NULL, &drc_playagain);
					SDL_RenderPresent(renderTarget);
				}
				else if (CheckForcusWithRect(xm, ym, drc_exit))
				{
					SDL_RenderCopy(renderTarget, exit2, NULL, &drc_exit);
					SDL_RenderPresent(renderTarget);
				}
				else
				{
					SDL_RenderCopy(renderTarget, playagain1, NULL, &drc_playagain);
					SDL_RenderCopy(renderTarget, exit1, NULL, &drc_exit);
					SDL_RenderPresent(renderTarget);
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				xm = m_event.button.x;
				ym = m_event.button.y;
				if (CheckForcusWithRect(xm, ym, drc_playagain))
				{
					*goalCount1 = 0;
					*goalCount2 = 0;
					initOpject(alCar1, 120, SCREEN_HEIGHT / 2 - CAR_HEIGHT, 90);
					initOpject(alCar2, SCREEN_WIDTH - 160 - CAR_WIDTH / 2, SCREEN_HEIGHT / 2 - CAR_HEIGHT, -90);
					initOpject(alBall, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 0);
					return 0;
				}
				if (CheckForcusWithRect(xm, ym, drc_exit))
				{
					return 1;
				}
			}
			break;
			}
		}
	}
}
