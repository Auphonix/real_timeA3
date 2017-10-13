#include "arena.h"

void arenaInit(Arena* arena, vec2 dimension){
    // dimensions
    arena->dimension = dimension;
}

void drawArena(Arena* arena){
    float x_len = arena->dimension.x;
    float y_len = arena->dimension.y;

    glBegin(GL_LINE_LOOP);
    glColor3f(0.337, 0.365, 0.275);
    glVertex2f(-x_len, y_len);
    glVertex2f(-x_len, -y_len);
    glVertex2f(x_len, -y_len);
    glVertex2f(x_len, y_len);
    glEnd();
}
