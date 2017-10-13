#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#if _WIN32
#  include <Windows.h>
#endif
#if __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <SDL2/SDL.h>

typedef struct Vec2{
    float x;
    float y;
} vec2;

typedef struct Vec3{
    float x;
    float y;
    float z;
} vec3;

float toRads(float angle);

#endif
