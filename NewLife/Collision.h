#pragma once
#include"Define.h"

void BallCollision(Opject *car, Opject *ball);
void carCollision(Opject *car, Opject *car2);
void applyForces(Opject *car, const int w, const int h);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);