#pragma once

float dot(float x1, float y1, float x2, float y2);
void normalize(float *x, float *y);
void reflect(float ix, float iy, float nx, float ny, float *rx, float *ry);
float length(float x, float y);
float radiansFromDegrees(float deg);
float degreesFromRadians(float rad);