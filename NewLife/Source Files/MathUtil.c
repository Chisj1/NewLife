
#include <math.h>
#include"..\Header Files\Define.h"
#include "..\Header Files\MathUtil.h"

//Tra ve x^2 + y^2
float dot(float x, float y)
{
  return x*x + y*y;
}

//Độ dài vector
float length(float x, float y)
{
  float lenSq = dot(x, y);
  float len = sqrtf(lenSq);
  return len;
}

//Chuẩn hóa vector
void normalize(float *x, float *y)
{
	float lenSq = dot(*x, *y);
	float len = sqrtf(lenSq);

	*x /= len;
	*y /= len;
}

//Chuyển Độ sang Rad
float radiansFromDegrees(float deg)
{
  return deg*(M_PI/180.0f);
}

