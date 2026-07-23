#pragma once
#include <cmath>
#include <cstdlib>
#include <cstdio>

typedef unsigned char byte;
typedef unsigned int uint;
typedef long double Ldbl;

#define _E        2.71828182845904523536L
#define PI        3.14159265358979323846L
#define PI_2      1.57079632679489661923L
#define PI_4      0.785398163397448309616L

#define _(x) *(x)
#define pw2(x) ((x)*(x))
#define pw3(x) ((x)*(x)*(x))

template<class T>
T iif(int cmp, T a, T b) { return cmp ? a : b; }
