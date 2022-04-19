
#include<SDL.h>
#include"Define.h"
#include"MathUtil.h"
#include<math.h> 
void initOpject(Opject *opjects, int posX, int posY)
{
	opjects->x = posX;
	opjects->y = posY;
	opjects->dx = 0;
	opjects->dy = 0;
	opjects->gdx = 0;
	opjects->gdy = 0;
	opjects->ang = 0;
}

int processEvents(SDL_Window *window, Opject *alCar1, Opject *alCar2, Opject *alBall)
{
	SDL_Event event;
	int done = 0;
	const float acc = 2.0f;
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
			//quit out of the game
			done = 1;
			break;
		}
	}


	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_A])
	{
		alCar1->ang -= 2;
	}
	if (state[SDL_SCANCODE_D])
	{
		alCar1->ang += 2;
	}
	if (state[SDL_SCANCODE_W])
	{
		float cdx = sinf(radiansFromDegrees(alCar1->ang))*acc*0.1;
		float cdy = -cosf(radiansFromDegrees(alCar1->ang))*acc*0.1;

		alCar1->dx += cdx;
		alCar1->dy += cdy;
	}
	if (state[SDL_SCANCODE_S])
	{
		float cdx = sinf(radiansFromDegrees(alCar1->ang))*0.1;
		float cdy = -cosf(radiansFromDegrees(alCar1->ang))*0.1;
		alCar1->dx += -cdx;
		alCar1->dy += -cdy;
	}

	if (state[SDL_SCANCODE_LEFT])
	{
		alCar2->ang -= 2;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		alCar2->ang += 2;
	}
	if (state[SDL_SCANCODE_UP])
	{
		float cdx = sinf(radiansFromDegrees(alCar2->ang))*acc*0.1;
		float cdy = -cosf(radiansFromDegrees(alCar2->ang))*acc*0.1;

		alCar2->dx += cdx;
		alCar2->dy += cdy;
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		float cdx = sinf(radiansFromDegrees(alCar2->ang))*0.1;
		float cdy = -cosf(radiansFromDegrees(alCar2->ang))*0.1;

		alCar2->dx += -cdx;
		alCar2->dy += -cdy;
	}


	float cdx = sinf(radiansFromDegrees(alBall->ang))*0.1;
	float cdy = -cosf(radiansFromDegrees(alBall->ang))*0.1;
	alBall->dx -= cdx;
	alBall->dy -= cdy;
	

	carCollision(alCar1, alCar2); 
	BallCollision(alCar1, alBall);
	BallCollision(alCar2, alBall);
	applyForces(alCar1, CAR_WIDTH * 2, CAR_HEIGHT * 2);
	applyForces(alCar2, CAR_WIDTH * 2, CAR_HEIGHT * 2);
	applyForces(alBall, BALL_RADIUS / 2 , BALL_RADIUS / 2 );

	return done;
}

void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball ,SDL_Texture *background)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderTarget);
	SDL_RenderCopy(renderTarget, background, NULL, NULL);
	//set the drawing color to white
	SDL_SetRenderDrawColor(renderTarget, 255, 255, 255, 255);

	SDL_Rect rect = { alCar->x, alCar->y, CAR_WIDTH * 4, CAR_HEIGHT * 4 };
	SDL_RenderCopyEx(renderTarget, car1, NULL, &rect, alCar->ang, NULL, 0);

	SDL_Rect rect2 = { alCar2->x, alCar2->y, CAR_WIDTH * 4, CAR_HEIGHT * 4};
	SDL_RenderCopyEx(renderTarget, car2, NULL, &rect2, alCar2->ang, NULL, 0);

	SDL_Rect rectBall = { alBall->x, alBall->y, BALL_RADIUS + 15, BALL_RADIUS + 15 };
	SDL_RenderCopyEx(renderTarget, ball, NULL, &rectBall, alBall->ang, NULL, 0);
	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderTarget);
}
