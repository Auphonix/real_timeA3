#include "catcher.h"

void catcherInit(Catcher* c, vec2 pos, float width, float v, vec3 color){
    c->pos = pos;
    c->width = width;
    c->vel = v;
    c->color = color;
}

void drawCatcher(Catcher* c){
    float half_wid = c->width / 2;

    float x_len = 0.02;
    float y_len = 0.05;

    glPushMatrix();
    glTranslatef(c->pos.x, c->pos.y, 0);
    glBegin(GL_QUADS);
    glColor3f(c->color.x, c->color.y, c->color.z);
    glVertex2f(-half_wid - x_len, y_len);
    glVertex2f(-half_wid - x_len, -y_len);
    glVertex2f(-half_wid + x_len, -y_len);
    glVertex2f(-half_wid + x_len, y_len);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(c->color.x, c->color.y, c->color.z);
    glVertex2f(half_wid - x_len, y_len);
    glVertex2f(half_wid - x_len, -y_len);
    glVertex2f(half_wid + x_len, -y_len);
    glVertex2f(half_wid + x_len, y_len);
    glEnd();
    glPopMatrix();
}


void moveCatcher(Time* t, Catcher* c){
    // Move horizontally
    c->pos.x += c->vel * t->time_passed;
    float extend = c->width / 2;

    float rx = c->pos.x + extend;
    float lx = c->pos.x - extend;
    //FIXME Change for dynamic arena size
    if(rx >= 1.5){ // HIT RIGHT WALL
        c->pos.x = 1.5 - extend;
        c->vel *= -1;
    }
    if(lx <= -1.5){
        c->pos.x = -1.5 + extend;
        c->vel *= -1;
    }
}
