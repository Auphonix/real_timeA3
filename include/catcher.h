#ifndef CATCHER_H
#define CATCHER_H

#include "helpers.h"
#include "time.h"

typedef struct Catcher{
    float width;
    vec2 pos;
    float vel; // 1D object
    vec3 color;
} Catcher;

void catcherInit(Catcher* c, vec2 pos, float width, float v, vec3 color);
void drawCatcher(Catcher* c);
void moveCatcher(Time* t, Catcher* c);
#endif
