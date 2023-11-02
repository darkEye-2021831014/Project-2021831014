////////////////////////////////////////////////////////////////////////////////////////
// Written By: Ashraful Islam
// Reg : 2021831014
// Project link on Github: https://github.com/darkEye-2021831014/Project-2021831014.git
////////////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
using namespace std;

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

// Global variables
bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Uint32 startTime;
Uint32 currentTime;
Uint32 elapsedTime;
const int radius = 10;
double newRadius = radius;

bool initializeWindow(void);
void processInput(void);

void drawCircle(int r);
void draw();

void destroyWindow(void);

int main(int argc, char **argv)
{
    // game loop control variable
    gameIsRunning = initializeWindow();
    startTime = SDL_GetTicks();

    while (gameIsRunning)
    {
        processInput();
        draw();
    }

    SDL_Log("Game Is Exiting!\n");
    // cleanup everything and exit
    destroyWindow();

    return 0;
}

// implimentation of all prototype functions

bool initializeWindow()
{
    // intialize sdl with video support
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // create sdl window
    window = SDL_CreateWindow(
        "Circle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0);

    if (!window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // create a SDL renderer to render graphics on window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        printf("Error: failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // everything is working fine
    return true;
}

void processInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameIsRunning = false;
            break;
        default:
            break;
        }
    }
}

void drawCircle(int r)
{
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    double angle = 0.0 * (M_PI / 180); // convert the angle into radian

    vector<SDL_Point> points;
    while (angle <= (360 * (M_PI / 180)))
    {
        SDL_Point point;
        point.x = centerX + r * cos(angle);
        point.y = centerY - r * sin(angle);
        points.push_back(point);

        angle += .3 * (M_PI / 180);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderDrawLines(renderer, points.data(), points.size());
}

void draw()
{
    // set renderer draw color(R,G,B,A) & clear the renderer with that color
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);

    currentTime = SDL_GetTicks();
    // calculate the time in milliseconds
    elapsedTime = (currentTime - startTime);

    // change the radius (by 2) of the circle after every 2 milliseconds
    if (elapsedTime % 2 == 0)
    {
        newRadius += 2;
        if ((newRadius * 2) >= SCREEN_HEIGHT || (newRadius * 2) >= SCREEN_WIDTH)
            newRadius = radius; // set the newRadius to initial radius
    }

    // draw a circle with newRadius as radius
    drawCircle(newRadius);

    // present the renderer(e.g draw the frame to the window)
    SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
