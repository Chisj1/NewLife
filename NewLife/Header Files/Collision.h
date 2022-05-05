#pragma once
#include"Define.h"

void BallCollision(Opject *car, Opject *ball);
void carCollision(Opject *car, Opject *car2);
void applyForces(Opject *car);
void applyForcesBall(Opject *ball, Opject *net);
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2);
int isGoal(Opject *ball);
void NetCollision(Opject *ball, Opject *net);