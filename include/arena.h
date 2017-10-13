#ifndef ARENA_H
#define ARENA_H

#include "helpers.h"

typedef struct Arena{
    vec2 dimension;
    vec2 P; // Momentum of wall
} Arena;

void arenaInit(Arena* arena, vec2 dimension);
void drawArena(Arena* arena);
#endif
