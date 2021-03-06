//==================================================================================================================|
// Created 2013.11.19 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _t3d_HELPER_H
#define _t3d_HELPER_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef glm::mat4 Mat4;
typedef glm::vec4 Vec4;
typedef glm::vec3 Vec3;

/**
	* \brief Represents a 2D rectangle with a point and size
	*/
template<typename T>
struct Rect2
{
	T x, y;
	T width, height;
};

typedef Rect2<int> Rect2i;
typedef Rect2<float> Rect2f;
typedef Rect2<double> Rect2d;


const double PI = 3.141592654;
const double TWO_PI = PI * 2.0;
const double PI_OVER_TWO = PI / 2.0;
const double THREE_PI_OVER_TWO = 3.0 * PI / 2.0;
const double PI_OVER_180 = 0.0174532925;
const double PI_UNDER_180 = 57.29577951;

double divideD(double a, double b);
float divideF(float a, float b);
int divideI(int a, int b);

double degToRad(double deg);
float degToRad(float deg);
int degToRad(int deg);
double radToDeg(double rad);
float radToDeg(float rad);
int radToDeg(int rad);

bool isEqual(float a, float b);
bool isEqual(double a, double b);

bool randBool();
int randInt();
int randInt(int a, int b);
double randFloat();
double randFloat(double a, double b);
double randClamped();

bool isPowerOfTwo(int value);

#endif

