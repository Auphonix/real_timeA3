#include "globals.h"

void initGlobals(Globals* g){
    g = (Globals *) malloc(sizeof(Globals));
    g->lastTime = 0;
}
