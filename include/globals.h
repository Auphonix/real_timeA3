#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

#include "helpers.h"
#include "player.h"

typedef struct Globals {
    // FPS
    unsigned int lastTime;
    unsigned int currentTime;
    int frames;
    float time_passed;

    Player* player;
} Globals;

void initGlobals(Globals* g);

#endif
