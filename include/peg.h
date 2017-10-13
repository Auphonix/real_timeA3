#ifndef PEG_H
#define PEG_H

#include "helpers.h"

typedef struct Peg {
    vec2 pos;
    vec2 vel;
    vec2 P; // Momentum
    float radius;
    float mass;
    float elasticity;
    GLUquadric *quadric; // Rendering
    int slices, loops; // Rendering
    vec3 color;
    bool moving;
    bool hit;
} Peg;

void pegInit(Peg* p, float num_pegs, float r, float m, float e, int slice, int loop, vec3 color);
void drawPegs(Peg *p, float num_pegs);
bool checkPegPosOverlap(Peg* pinit, float x, float y, int i, float r);

#endif
