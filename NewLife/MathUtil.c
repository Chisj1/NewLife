
#include <math.h>
#include "MathUtil.h"
#include"Define.h"
float dot(float x1, float y1, float x2, float y2)
{
  return x1*x2 + y1*y2;
}

float length(float x, float y)
{
  float lenSq = dot(x, y, x, y);
  float len = sqrtf(lenSq);
  return len;
}

void normalize(float *x, float *y)
{
	float lenSq = dot(*x, *y, *x, *y);
	float len = sqrtf(lenSq);

	*x /= len;
	*y /= len;
}

void reflect(float ix, float iy, float nx, float ny, float *rx, float *ry)
{
  //I - 2.0 * dot(N, I) * N
  float ni = dot(nx, ny, ix, iy);
  float rhs[] = { 2*ni*nx, 2*ni*ny };
  
  *rx = ix - rhs[0];
  *ry = iy - rhs[1];
}

float radiansFromDegrees(float deg)
{
  return deg*(M_PI/180.0f);
}

float degreesFromRadians(float rad)
{
  return rad/(M_PI/180.0f);
}