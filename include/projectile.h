#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "helpers.h"
#include "player.h"
#include "time.h"
#include "arena.h"
#include "peg.h"

typedef enum renderMode {WIRE, FILL} renderMode;

typedef struct Projectile{
    vec2 pos;
    vec2 posi; // initial pos
    float speed;
    vec2 vel;
    float radius;
    float mass;
    float elasticity;
    GLUquadric *quadric; // Rendering
    int slices, loops; // Rendering
    vec3 color;
    bool moving;
    float multiplier;
    renderMode mode;
} Projectile;

void projectileInit(Projectile* p, float speed, vec2 pos, vec2 vel, float r, float m,
    float e, int slice, int loop, vec3 color);
void drawBall(Projectile *p);
void shootBall(Player* pl, Projectile* pr);
vec2 calcPos(Player* pl, Projectile* pr);
void moveBall(Time* t, Peg* pe, Projectile* pr);
int checkCollision(Arena* a, Peg* pe, Projectile* pr, float num_pegs, float* multiplier);
void collisionReactionParticles2DprojNormal(Projectile *pr, Peg *pe);
void resetBall(Projectile* pr);
#endif
