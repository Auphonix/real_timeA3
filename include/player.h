#ifndef PLAYER_H
#define PLAYER_H

#include "helpers.h"

typedef struct Player{
    vec2 pos;
    float angle;
} Player;

void playerInit(Player* player, float angle, vec2 pos);
void drawPlayer(Player* player);
void movePlayer(Player* player);

#endif
