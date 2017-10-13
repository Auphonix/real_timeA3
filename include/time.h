#ifndef TIME_H
#define TIME_H

#include "helpers.h"

typedef struct Time {
    unsigned int lastTime;
    unsigned int currentTime;
    int frames;
    float time_passed;
} Time;

void timeInit(Time* t, float lastTime);
#endif
