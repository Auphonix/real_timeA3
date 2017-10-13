#include "globals.h"

void initGlobals(Globals* g, bool reset){

    /*----------- TIME INIT ----------*/
    // time
    // last time
    timeInit(&g->time, 0);



    /*--------- AREA INIT ------------*/
    //AREA
    //dimensions
    arenaInit(&g->arena, (vec2){1.5, 1.5});



    /*-------- PLAYER INIT ------------*/
    // player
    // angle
    // pos
    playerInit(&g->player, 180, (vec2){0, 1.5});



    /*----- PROJECTILE INIT --------- */
    // projectile
    // Speed
    // initial ball render at (0,2.5) so not on screen
    // vel
    // radius
    // mass
    // elasticity
    // slice
    // loop
    // color
    projectileInit(&g->projectile, 2, (vec2) {0, 2.5}, (vec2) {0, 0}, 0.05, 2, 1.0, 10, 3, (vec3) {1, 0.075, 0.0});


    /*--------- PEG SETUP -----------*/
    // if(reset){
    //     Peg* p = g->pegs;
    //     for (int i = 1; i < g->num_pegs; i++){
    //
    //     }
    // }
    // RANDOM LIMITS
    // const int RMAX = 60; // MAX 100
    // const int RMIN = 40;
    // g->num_pegs = rand() % RMAX + RMIN; // between 20 and 40
    g->num_pegs = 100;
    // vec2 peg_positions[20] = {{-1, 0.8}, {-1, 0.6}, {-1, 0.4}, {-1, 0.2}, {-1, 0},
    //                     {-1, -0.2}, {-1, -0.4}, {-1, -0.6}, {-1, -0.8}, {-1, -1},
    //                     {-1, -1.2}, {-0.8, -1.2}, {-0.6, -1.2}, {-0.4, -1.2}, {-0.2, -1.2},
    //                     {0, -1.2}, {0.2, -1.2}, {0.4, -1.2}, {0.6, -1.2}, {0.8, -1.2}};
    printf("Number of pegs in this game: %i\n", g->num_pegs);

    /*--------- PEG INIT ------------*/
    // pointer to 1st peg
    // initial ball render at (0,2.5) so not on screen
    // radius
    // mass
    // elasticity
    // slice
    // loop
    // color
    //(vec3) {0.25, 0.5, 0}
    pegInit(g->pegs, g->num_pegs, 0.05, 30, 1.0, 10, 3, (vec3) {0.337, 0.365, 0.275});


    /*----------- GAME INIT ----------*/
    g->total_score = 0;
    g->ball_score = 0;
    g->balls = 10;
    g->multiplier = 1;
    printf("Game started with %i balls remaining\n", g->balls);
}
