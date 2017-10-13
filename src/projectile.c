#include "projectile.h"
#include <math.h>

#define GRAVITY 3;
#define PEG_POINT 10;

void projectileInit(Projectile* p, float speed, vec2 pos, vec2 vel, float r, float m, float e, int slice, int loop, vec3 color){
    p->pos = pos;
    p->posi = pos;
    p->speed = speed;
    p->vel = vel;
    p->radius = r;
    p->mass = m;
    p->elasticity = e;
    p->quadric = gluNewQuadric();
    p->slices = slice;
    p->loops = loop;
    p->color = color;
}

void drawBall(Projectile *p){
    if (!p->quadric) {
        fprintf(stderr, "drawBall: no quadric for disk\n");
        exit(1);
    }

    glPushMatrix();
    if(p->mode == WIRE){
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_NORMALIZE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glColor3f(p->color.x, p->color.y, p->color.z);
    glTranslatef(p->pos.x, p->pos.y, 0);
    gluDisk(p->quadric, 0.0, p->radius, p->slices, p->loops);
    glPopMatrix();
}

void shootBall(Player* pl, Projectile* pr){
    pr->pos = calcPos(pl, pr);

    printf("angle: %f\n", pl->angle);
    float angle_rads = toRads(pl->angle);
    pr->vel.x =  sin((2 * M_PI) - angle_rads) * pr->speed;
    pr->vel.y =  cos((2 * M_PI) - angle_rads) * pr->speed;
}

vec2 calcPos(Player* pl, Projectile* pr){
    // 0.15 = gun height
    float angle_rads = toRads(pl->angle);
    float x = sin((2 * M_PI) - angle_rads) * 0.3;
    float y = cos((2 * M_PI) - angle_rads) * 0.3 + 1.5;

    return (vec2) {x, y};
}

void moveBall(Time* t, Peg* pe, Projectile* pr){
    if(!pr->moving) return;
    float g = GRAVITY;
    // move ball using projectile motion
    /* horizontal */
    pr->pos.x += pr->vel.x * t->time_passed;

    /* vertical */
    float vyi = pr->vel.y;
    // pr->pos.y = pr->posi.y + vyi * t->time_passed - (1/2) * g * pow(t->time_passed, 2);
    pr->pos.y += t->time_passed * pr->vel.y;
    pr->vel.y = vyi - g * t->time_passed;

    // pr->vel.y += -g * t->time_passed;
    // pr->pos.x += t->time_passed * pr->vel.x;
    // pr->pos.y += t->time_passed * pr->vel.y;
    // for (int i = 0; i < 20; i++){
    //     pe->pos.x += t->time_passed * pe->vel.x;
    //     pe->pos.y += t->time_passed * pe->vel.y;
    //     pe++;
    // }
}

// Returns the accumulated score
int checkCollision(Arena* a, Peg* pe, Projectile* pr, float num_pegs, float* multiplier){
    float dist_x, dist_y, sum_dist, sum_radii;
    float score = 0;

    // Elastic mode
    // Collisions amongst disks?
    for (int i = 0; i < num_pegs; i++) {
        // v1i = disk[i].velocity;
        // m1 = disk[i].mass;
        sum_radii = pr->radius + pe->radius;
        dist_x = pow(pr->pos.x - pe->pos.x, 2);
        dist_y = pow(pr->pos.y - pe->pos.y, 2);
        sum_dist = sqrt(dist_x + dist_y);
        if (sum_dist <= sum_radii) { // Collides
            if(sum_dist < sum_radii){ // Overlap
                // printf("--------- START SEGMENT --------\n");
                // printf("pr.x: %f\n", pr->pos.x);
                // printf("pr.y: %f\n", pr->pos.y);
                // printf("pe.x: %f\n", pe->pos.x);
                // printf("pe.y: %f\n", pe->pos.y);
                // printf("radii: %f\n\n", sum_radii);
                // mag of a
                vec2 n;
                n.x = pe->pos.x - pr->pos.x;
                n.y = pe->pos.y - pr->pos.y;

                //Normalize
                float mag = sqrt(pow(n.x, 2) + pow(n.y, 2));
                n.x = n.x / mag * (sum_radii + 0.001);
                n.y = n.y / mag * (sum_radii + 0.001);

                // Fix position
                // printf("pe.x: %f\n", pe->pos.x + vector.x);
                pr->pos.x = pe->pos.x - n.x;
                pr->pos.y = pe->pos.y - n.y;


                // printf("pos.x: %f\n", pr->pos.x);
                // printf("pos.y: %f\n", pr->pos.y);
                // printf("--------- END SEGMENT --------\n\n");

            }
            // DEALWITH COLLISION
            collisionReactionParticles2DprojNormal(pr, pe);
            //{0.82, 0.59, 1};
            pe->color = (vec3){.533, .737, .078};

            // Update score
            if(!pe->hit){
                score += *multiplier * PEG_POINT;
                printf("multiplier: %f\n", *multiplier);
                printf("pin value: %f\n", score);
                *multiplier += 0.5;

                pe->hit = true;
            }

        }
        pe++; // Next peg
    }

    // wall bounce on x plane
    if ((pr->pos.x - pr->radius) < -a->dimension.x ||
    (pr->pos.x + pr->radius) > a->dimension.x) {
        a->P.x += pr->mass * pr->vel.x * 2;

        if(pr->vel.x < 0){ // Left wall hit
        pr->pos.x = -a->dimension.x + pr->radius; // Prevent rebounce
      }
        else if(pr->vel.x > 0){ // Left wall hit
        pr->pos.x = a->dimension.x - pr->radius; // Prevent rebounce
      }
        pr->vel.x *= -1;
        // printf("wall bounce horizontal\n");
    }

    // Wall bounce on y plane - only check bottom as ball can't hit top
    if ((pr->pos.y - pr->radius) <= -a->dimension.y) {
        a->P.y += pr->mass * pr->vel.y * 2;

        if(pr->vel.y < 0){ // bottom wall hit
            resetBall(pr);
            score = -1;
            *multiplier = 1;
        }
        // printf("wall bounce vertical\n");
    }

    // Change new initial position
    pr->posi.x = pr->pos.x;
    pr->posi.y = pr->pos.y;

    // Return score
    return score;
}

void resetBall(Projectile* pr){
    pr->pos.y = 2.5;
    pr->vel.y = 0;
    pr->vel.x = 0;
    pr->moving = false;
}


void collisionReactionParticles2DprojNormal(Projectile *pr, Peg *pe) {
    vec2 n;
    float n_mag;
    float projnv1, projnv2;
    float m1, m2, v1i, v2i, v1f, v2f;

    // Normal vector n between centres.
    // nx = x2 - x1
    n.x = pe->pos.x - pr->pos.x;
    n.y = pe->pos.y - pr->pos.y;

    // Normalise n.
    // magnitude  = sqrt(nx^2 + ny^2)
    n_mag = sqrt(n.x * n.x + n.y * n.y);
    n.x /= n_mag;
    n.y /= n_mag;

    // Vector projection of velocity in n direction.
    //vf = nx * vix + ny * viy
    projnv1 = n.x * pr->vel.x + n.y * pr->vel.y;
    projnv2 = n.x * pe->vel.x + n.y * pe->vel.y;

    // Use 1D equations to calculate final velocities in n direction.
    v1i = projnv1;
    v2i = projnv2;
    m1 = pr->mass;
    m2 = pe->mass;
    printf("v1i: %f\n", v1i);
    v1f = ((m1 - m2) / (m1 + m2)) * v1i + ((2.0 * m2) / (m1 + m2)) * v2i;
    printf("v1f: %f\n", v1f);
    // Used for peg
    //v2f = 2.0 * m1 / (m1 + m2) * v1i + (m2 - m1) / (m1 + m2) * v2i;

    /* Vector addition to solve for final velocity. */
    pr->vel.x = (pr->vel.x - v1i * n.x) + v1f * n.x;
    pr->vel.y = (pr->vel.y - v1i * n.y) + v1f * n.y;
    pe->vel.x = 0;
    pe->vel.y = 0;
}
