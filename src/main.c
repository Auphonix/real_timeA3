// Make sure you add the SDL2 library to the library list when compiling: -lSDL2
// gcc -o a3 main.c -F /Library/Frameworks -framework Carbon -framework OpenGL -framework SDL2 -framework GLUT -Wno-deprecated

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if _WIN32
#  include <Windows.h>
#endif
#if __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <SDL2/SDL.h>

#include "globals.h"

Globals g;

SDL_Window *mainWindow = 0;
const int MAX_DOMAIN = 2; // Used for window dimensions -2 -> 2
const int MIN_DOMAIN = -2;

void init()
{
    initGlobals(&g, false);
}

void drawAxes(float length)
{
    // RED (X-axis)
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);
    glEnd();

    // GREEN (Y-axis)
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    glEnd();

    // NO REASON TO HAVE Z AXIS IN 2D GAME
    // BLUE (Z-axis)
    // glBegin(GL_LINE_STRIP);
    // glColor3f(0, 0, 1);
    // glVertex3f(0, 0, 0);
    // glVertex3f(0, 0, length);
    // glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // AXIS
    if(g.toggles.axis)
    drawAxes(1.5);

    // Draw Arena
    drawArena(&g.arena);

    // Draw pegs
    drawPegs(g.pegs, g.num_pegs);

    // Draw Player
    drawPlayer(&g.player);

    drawBall(&g.projectile);

    SDL_GL_SwapWindow(mainWindow);

    // Always check for GL errors at least once per frame
    int err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("display: %s\n",gluErrorString(err));
    }

}

// MAIN TIME LOOP
void update()
{
    int t = SDL_GetTicks();
    g.time.time_passed = (float)(t - g.time.currentTime) / (float)1000;
    g.time.currentTime = t;
    if (g.time.currentTime > g.time.lastTime + 1000) {
        if(g.toggles.console) // ONLY PRINT ON TOGGLE
        printf("FPS: %i\n", g.time.frames);
        g.time.lastTime = g.time.currentTime;
        g.time.frames = 0;
    }

    // move ball using numerical integration
    moveBall(&g.time, g.pegs, &g.projectile);
    float tmp_score = checkCollision(&g.arena, g.pegs, &g.projectile, g.num_pegs, &g.multiplier);
    if(tmp_score == -1) {
        g.total_score += g.ball_score;
        printf("Score for that ball is: %i\n", g.ball_score);
        printf("Total score is: %i\n\n", g.total_score);
        g.ball_score = 0;
    }
    else g.ball_score += tmp_score;

    g.time.frames++;
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(MIN_DOMAIN, MAX_DOMAIN, MIN_DOMAIN, MAX_DOMAIN, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool handleKeyDown(SDL_Keysym *key)
{
    // Handle key press events here
    switch (key->sym)
    {
        case SDLK_ESCAPE:
        return true;
        break;

        // TOGGLE CONSOLE
        case SDLK_c:
        g.toggles.console = !g.toggles.console;
        break;

        // TOGGLE AXIS
        case SDLK_a:
        g.toggles.axis = !g.toggles.axis;
        break;

        case SDLK_q:
        return true;
        break;

        case SDLK_w:
        if(g.projectile.mode == WIRE) g.projectile.mode = FILL;
        else g.projectile.mode = WIRE;
        break;

        case SDLK_r: // RESTART
        initGlobals(&g, true);
        break;

        case SDLK_LEFT:
        if(g.player.angle > 96)
        g.player.angle -= 5;
        break;
        case SDLK_RIGHT:
        if(g.player.angle < 264)
        g.player.angle += 5;
        break;

        // Shoot ball
        case SDLK_SPACE:
        if(!g.projectile.moving){
            if(g.balls > 0){
                printf("You have %i balls remaining\n", --g.balls);
                shootBall(&g.player, &g.projectile);
                g.projectile.moving = true;
                g.projectile.posi.x = g.projectile.pos.x;
                g.projectile.posi.y = g.projectile.pos.y;
            }
        }
        break;

        default:
        break;
    }

    return false;
}

bool handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            return handleKeyDown(&event.key.keysym);
            break;

            case SDL_QUIT:
            return true;
            break;

            case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
                // RESIZE WINDOW
                case SDL_WINDOWEVENT_RESIZED:
                if (event.window.windowID == SDL_GetWindowID(mainWindow))
                SDL_SetWindowSize(mainWindow, event.window.data1, event.window.data2);
                reshape(event.window.data1, event.window.data2);
                break;

                // WINDOW CLOSE
                case SDL_WINDOWEVENT_CLOSE:
                return true;
                break;
                default:
                break;
            }
            break;
            default:
            break;
        }
    }

    return false;
}

void mainLoop(){
    bool done = false;
    // Main event and display loop goes here
    while(!done){
        done = handleEvents();
        update();
        display();
    }
}


int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return EXIT_SUCCESS;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    mainWindow = SDL_CreateWindow("A3 - Peggle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 850, 850, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!mainWindow) {
        fprintf(stderr, "Failed to create a window: %s\n", SDL_GetError());
        return EXIT_SUCCESS;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GLContext mainGLContext = SDL_GL_CreateContext(mainWindow);
    if (mainGLContext == 0) {
        fprintf(stderr, "Unable to get OpenGL context: %s\n", SDL_GetError());
        return EXIT_SUCCESS;
    }

    if (SDL_GL_MakeCurrent(mainWindow, mainGLContext) != 0) {
        fprintf(stderr, "Unable to make OpenGL context current: %s\n", SDL_GetError());
        return EXIT_SUCCESS;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    SDL_Renderer* renderer = SDL_CreateRenderer(mainWindow, -2, 0);
    SDL_SetRenderDrawColor(renderer, 235, 239, 226, 255);

    // Clear screen to colour
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    int w, h;
    SDL_GetWindowSize(mainWindow, &w, &h);
    reshape(w, h);

    init();

    mainLoop();

    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
