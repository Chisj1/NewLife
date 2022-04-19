#include"MathUtil.h"
#include"Collision.h"
#include"Define.h"

void BallCollision(Opject *car, Opject *ball)
{
	const acc = 0.1f;
	const float wBall = BALL_RADIUS, hBall = BALL_RADIUS;
	const float wCar = CAR_WIDTH * 2, hCar = CAR_HEIGHT * 2;


	if (collide2d(car->x, car->y, ball->x, ball->y, wCar, hCar, wBall, hBall))
	{
		float vx = ball->x - car->x;
		float vy = ball->y - car->y;
		float carSpeed = length(car->dx, car->dy);
		float ballSpeed = length(ball->dx, ball->dy);

		if (carSpeed < 1.0f)
		{
			carSpeed = 1.0f;
		}
		normalize(&vx, &vy);

		car->gdx += -vx * carSpeed;
		car->gdy += -vy * carSpeed;
		ball->gdx += vx * carSpeed * 5;
		ball->gdy += vy * carSpeed * 5;

	}

}

void carCollision(Opject *car, Opject *car2)
{
	const float w = CAR_WIDTH * 4 - 2 , h = CAR_HEIGHT * 4 + 1;
	if (collide2d(car->x, car->y, car2->x, car2->y, w, h, w, h))
	{
		float vx = car2->x - car->x;
		float vy = car2->y - car->y;
		float car1Speed = length(car->dx, car->dy);
		float car2Speed = length(car2->dx, car2->dy);


		if (car1Speed < 1.0f)
		{
			car1Speed = 1.0f;
		}
		if (car2Speed < 1.0f)
		{
			car2Speed = 1.0f;
		}

		normalize(&vx, &vy);

		car->gdx += -vx * car2Speed;
		car->gdy += -vy * car2Speed;
		car2->gdx += vx * car1Speed;
		car2->gdy += vy * car1Speed;
	}
}

void applyForces(Opject *car, const int w, const int h)
{
	if (car->x  < 22 + w )
	{
		car->x = 22 + 5 + w;
		if (car->gdx < 0)
		{
			car->gdx *= -1;
		}
		if (car->dx < 0)
		{
			car->dx *= -1;
		}
	}
	if (car->x  > SCREEN_WIDTH - 45 - w)
	{
		car->x = SCREEN_WIDTH - 45 - 5 - w;
		if (car->gdx > 0)
		{
			car->gdx *= -1;
		}
		if (car->dx > 0)
		{
			car->dx *= -1;
		}
	}
	if (car->y + h < 80 )
	{
		car->y =  80 + 5 - h;
		if (car->gdy < 0)
		{
			car->gdy *= -1;
		}
		if (car->dy < 0)
		{
			car->dy *= -1;
		}
	}
	if (car->y > SCREEN_HEIGHT - 30 - h)
	{
		car->y = SCREEN_HEIGHT - 30 - 5 - h;
		if (car->gdy > 0)
		{
			car->gdy *= -1;
		}
		if (car->dy > 0)
		{
			car->dy *= -1;
		}
	}

	car->x += car->dx;
	car->y += car->dy;
	car->x += car->gdx;
	car->y += car->gdy;
	car->dx *= 0.96;
	car->dy *= 0.96;
	car->gdx *= 0.92;
	car->gdy *= 0.92;
}




int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}
