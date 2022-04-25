﻿#include"..\Header Files\MathUtil.h"
#include"..\Header Files\Define.h"
#include"..\Header Files\Collision.h"
#include<SDL.h>

//Phát hiện va chạm giữa 2 vật thể
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

//Xử lý va chạm giữa bóng và xe, coi m Xe = 4 m bóng
void BallCollision(Opject *car, Opject *ball)
{
	//Gán giá trị 2 chiều xe và bóng
	const float wBall = BALL_RADIUS, hBall = BALL_RADIUS;
	const float wCar = CAR_WIDTH * 2, hCar = CAR_HEIGHT * 2;


	if (collide2d(car->x, car->y, ball->x, ball->y, wCar, hCar, wBall, hBall))
	{
		//Vector giữa 2 vật
		float vx = ball->x - car->x;
		float vy = ball->y - car->y;
		float carSpeed = length(car->dx, car->dy);

		//Nếu vận tốc xe < 1 thì cho = 1 để chương trình ko phải nhận phép tính quá nhỏ
		if (carSpeed < 1.0f)
		{
			carSpeed = 1.0f;
		}

		//Chuẩn hóa
		normalize(&vx, &vy);

		//Khi đâm vào xe sẽ bị lùi lại còn bóng sẽ tiến lên = 4 lần xe lùi lại
		
		car->gdx += -vx * carSpeed;
		car->gdy += -vy * carSpeed;
		ball->gdx += vx * carSpeed * 4.0f;
		ball->gdy += vy * carSpeed * 4.0f;

	}

}

//Như hàm ở trên chỉ khác là va chạm với 2 xe và 2 xe có cùng khối lượng
void carCollision(Opject *car, Opject *car2)
{
	const float w = CAR_WIDTH * 2 + 20, h = CAR_HEIGHT * 2 - 25;
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

void applyForces(Opject *car)
{
	//Gia tốc của vận tốc và Lực
	const float accD = 0.96f;
	const float accGd = 0.9f;
	//Giới hạn khung hình cho xe

	if (car->x < 60)//Bên trái
	{
		//Khi xe đến điểm giới hạn sẽ bị bật lại
		car->x = 60 + 5;
		
		//Vector vận tốc và lực sẽ bị đảo chiều sau va chạm
		if (car->gdx < 0)
		{
			car->gdx *= -1;
		}
		if (car->dx < 0)
		{
			car->dx *= -0.7f;
		}
	}
	if (car->x > SCREEN_WIDTH - 110)//Bên Phải
	{
		car->x = SCREEN_WIDTH - 110 - 5;
		if (car->gdx > 0)
		{
			car->gdx *= -1;
		}
		if (car->dx > 0)
		{
			car->dx *= -0.7f;
		}
	}
	if (car->y < 18)//Ở trên
	{
		car->y = 18 + 5;
		if (car->gdy < 0)
		{
			car->gdy *= -1;
		}
		if (car->dy < 0)
		{
			car->dy *= -0.7f;
		}
	}
	if (car->y > SCREEN_HEIGHT - 80)//Ở dưới
	{
		car->y = SCREEN_HEIGHT - 80 - 5;
		if (car->gdy > 0)
		{
			car->gdy *= -1;
		}
		if (car->dy > 0)
		{
			car->dy *= -0.7f;
		}
	}

	//Tính toán tọa độ của x y qua mỗi khung hình
	car->x += car->dx;//Thành phần vận tốc
	car->y += car->dy;
	car->x += car->gdx;//Thành phần lực
	car->y += car->gdy;

	//Vận tốc xe giảm dần
	car->dx *= accD;
	car->dy *= accD;
	car->gdx *= accGd;
	car->gdy *= accGd;
}


int isGoal(Opject *ball)
{
	if ((ball->y >= 330 && ball->y <= 560))
	{
		if (ball->x <= 25)
		{
			return 1;
		}

		else if (ball->x >= 1500)
		{
			return 1;
		}

	}
	return 0;
}

void applyForcesBall(Opject *ball)
{
	const float accD = 0.95f;
	const float accGd = 0.97f;

	if (ball->y >= 330 && ball->y <= 560)
	{
		if (ball->x <= 25)
		{
			ball->dx *= accD * 0.1f;
			ball->dy *= accD * 0.1f;
			ball->gdx *= accGd * 0.1f;
			ball->gdy *= accGd * 0.1f;

		}
		else if (ball->x >= 1500)
		{
			ball->dx *= accD * 0.1f;
			ball->dy *= accD * 0.1f;
			ball->gdx *= accGd * 0.1f;
			ball->gdy *= accGd * 0.1f;

		}
	}
	else
	{
		if (ball->x < 42)
		{
			ball->x = 42 + 5;
			if (ball->gdx < 0)
			{
				ball->gdx *= -1;
			}
			if (ball->dx < 0)
			{
				ball->dx *= -1;
			}
		}
		if (ball->x > SCREEN_WIDTH - 90 - BALL_RADIUS / 2)
		{
			ball->x = SCREEN_WIDTH - 90 - BALL_RADIUS / 2 - 5;
			if (ball->gdx > 0)
			{
				ball->gdx *= -1;
			}
			if (ball->dx > 0)
			{
				ball->dx *= -1;
			}
		}
		if (ball->y < 18)
		{
			ball->y = 18 + 5;
			if (ball->gdy < 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy < 0)
			{
				ball->dy *= -1;
			}
		}
		if (ball->y < 565 && ball->y > 565 - BALL_RADIUS - 10 && ball->x < 100)//Khung thành bên trái phía dưới
		{
			ball->y = 565 + 5;
			if (ball->gdy < 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy < 0)
			{
				ball->dy *= -1;
			}
		}
		if (ball->y < 565 && ball->y > 565 - BALL_RADIUS - 10 && ball->x > 1450)//Khung thành bên phải phía dưới
		{
			ball->y = 565 + 5;
			if (ball->gdy < 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy < 0)
			{
				ball->dy *= -1;
			}
		}

		if (ball->y > SCREEN_HEIGHT - 70 - BALL_RADIUS / 2)
		{
			ball->y = SCREEN_HEIGHT - 70 - BALL_RADIUS / 2 - 5;
			if (ball->gdy > 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy > 0)
			{
				ball->dy *= -1;
			}
		}

		if (ball->y > 330 - BALL_RADIUS - 10 && ball->y < 330 && ball->x < 100) //Khung thành bên trái phía trên
		{
			ball->y = 330 - BALL_RADIUS - 10 - 5;
			if (ball->gdy > 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy > 0)
			{
				ball->dy *= -1;
			}
		}
		if (ball->y > 330 - BALL_RADIUS - 10 && ball->y < 330 && ball->x > 1450)// Khung thành bên phải phía dưới
		{
			ball->y = 330 - BALL_RADIUS - 10 - 5;
			if (ball->gdy > 0)
			{
				ball->gdy *= -1;
			}
			if (ball->dy > 0)
			{
				ball->dy *= -1;
			}
		}


	}

	ball->x += ball->dx;
	ball->y += ball->dy;
	ball->x += ball->gdx;
	ball->y += ball->gdy;
	ball->dx *= accD;
	ball->dy *= accD;
	ball->gdx *= accGd;
	ball->gdy *= accGd;
}

