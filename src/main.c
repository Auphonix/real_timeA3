// Make sure you add the SDL2 library to the library list when compiling: -lSDL2
// gcc -o a3 main.c -F /Library/Frameworks -framework Carbon -framework OpenGL -framework SDL2 -framework GLUT -Wno-deprecated

#include "globals.h"

Globals* g;

SDL_Window *mainWindow = 0;
const int MAX_DOMAIN = 2; // Used for window dimensions -2 -> 2
const int MIN_DOMAIN = -2;

void init()
{
    initGlobals(g);
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

void drawArena(){

    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);
    glVertex2f(-1.5, -1.5);
    glVertex2f(-1.5, 1.5);
    glVertex2f(1.5, 1.5);
    glVertex2f(1.5, -1.5);
    glEnd();
}

void movePlayer(){
    glTranslatef(0, 1.5, 0);
    glRotatef(180, 0, 0, 1);
}

void drawPlayer(){
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 1);
    glVertex2f(-0.02, 0);
    glVertex2f(-0.02, 0.15);
    glVertex2f(0.02, 0.15);
    glVertex2f(0.02, 0);
    glEnd();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // AXIS
    drawAxes(1.5);

    // Draw Arena
    drawArena();

    // Draw Player

    movePlayer();
    drawPlayer();

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
    g->time_passed = (float)(t - g->currentTime) / (float)1000;
    g->currentTime = t;
    if (g->currentTime > g->lastTime + 1000) {
        printf("FPS: %i\n", g->frames);
        g->lastTime = g->currentTime;
        g->frames = 0;
    }

    g->frames++;
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

        case SDLK_q:
        return true;
        break;

        case SDLK_LEFT:
        printf("left is pressed\n");
        break;
        case SDLK_RIGHT:
        printf("left is pressed\n");
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


    mainWindow = SDL_CreateWindow("A3 - Peggle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

    int w, h;
    SDL_GetWindowSize(mainWindow, &w, &h);
    reshape(w, h);

    init();

    mainLoop();

    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
