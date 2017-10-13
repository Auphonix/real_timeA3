#include <stdlib.h>
#include <stdio.h>
#include "player.h"

void playerInit(Player* player, float angle, vec2 pos){
    player->angle = angle;
    player->pos = pos;
}

void drawPlayer(Player* player){
    glPushMatrix();
    movePlayer(player);
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.337, 0.365, 0.275);
    glVertex2f(-0.05, 0);
    glVertex2f(-0.05, 0.3);
    glVertex2f(0.05, 0);
    glVertex2f(0.05, 0.3);
    glEnd();
    glPopMatrix();
}

void movePlayer(Player* player){
    glTranslatef(player->pos.x, player->pos.y, 0);
    glRotatef(player->angle, 0, 0, 1);
}
