#include "peg.h"

void pegInit(Peg* p, float num_pegs, float r, float m, float e, int slice, int loop, vec3 color){
    Peg* pinit = p;
    for(int i = 0; i < num_pegs; i++ ){
        // RANDOM POSITIONS
        float XMAX = 2.7;
        float XMIN = -1.35;
        float YMIN = -1.2;
        float YMAX = 2;

        bool safe = false;
        float x, y;
        while(!safe){
            x = (float)rand()/(float)(RAND_MAX/XMAX) + XMIN;
            y = (float)rand()/(float)(RAND_MAX/YMAX) + YMIN;

            safe = checkPegPosOverlap(pinit, x, y, i, r);

        }
        p->pos = (vec2) {x, y};
        p->vel = (vec2) {0, 0};
        p->radius = r;
        p->mass = m;
        p->elasticity = e;
        p->quadric = gluNewQuadric();
        p->slices = slice;
        p->loops = loop;
        p->color = color;
        p++; // Go to next array element
    }
}

bool checkPegPosOverlap(Peg* pinit, float x, float y, int i, float r){

    Peg* p_inc = pinit;// Get initial peg

    for(int j = 0; j < i; j++){ // Check existing postion
        float dist_x = pow(x - p_inc->pos.x, 2);
        float dist_y = pow(y - p_inc->pos.y, 2);
        float sum_dist = sqrt(fabs(dist_x + dist_y));
        if(sum_dist <= r * 4){ // Peg overlap has occured
            return false;
        }
        p_inc++;
    }
    return true;
}

void drawPegs(Peg* p, float num_pegs){
    for(int i = 0; i < num_pegs; i++){
        if (!p->quadric) {
            fprintf(stderr, "drawBall: no quadric for disk\n");
            exit(1);
        }

        glPushMatrix();
        glColor4f(p->color.x, p->color.y, p->color.z, 1);
        glTranslatef(p->pos.x, p->pos.y, 0);
        glScalef(1, 1, 1);
        gluDisk(p->quadric, 0.0, p->radius, p->slices, p->loops);
        glPopMatrix();
        p++;
    }
}
