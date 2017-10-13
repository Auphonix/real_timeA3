#ifndef GLOBALS_H
#define GLOBALS_H

#include "helpers.h"
#include "player.h"
#include "time.h"
#include "projectile.h"
#include "arena.h"
#include "peg.h"
#include "catcher.h"

typedef struct Toggles {
    bool axis;
    bool console;
} Toggles;

typedef struct Globals {
    Toggles toggles;
    Time time;
    Player player;
    Arena arena;
    Projectile projectile;
    Catcher catcher;

    int num_pegs;
    Peg pegs[100];


    // Game
    int total_score;
    int ball_score;
    int balls;
    float multiplier;


} Globals;

void initGlobals(Globals* g, bool reset);

#endif
