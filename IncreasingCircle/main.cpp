/////////////////////////////////////////////////////////////////////////////
// Written By: Ashraful Islam
// Reg : 2021831014
// Project link on Github: https://github.com/darkEye-2021831014/Project-2021831014.git



#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
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
const int radius = 1;
double newRadius = radius;

bool initializeWindow(void)
{
    // Initialize SDL with video support
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Create an SDL window
    window = SDL_CreateWindow(
        "Ashraful Islam-2021831014",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Create an SDL renderer for rendering graphics in the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }
    return true;
}

void process_input(void)
{
    // Poll SDL events (e.g., window close)
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameIsRunning = false; // Exit the game loop
            break;

        default:
            break;
        }
    }
}


void drawCircle(int r)
{
    vector<SDL_Point> p;
    int centerX = (SCREEN_WIDTH/2);
    int centerY = (SCREEN_HEIGHT/2);
    SDL_Point point;
    int part = 0, y =centerY, x = 0;
    while(part != 4)
    {
        if(part == 0 || part == 3) 
        {
            y--;
            x = centerX + sqrt(pow(r,2) - pow((y-centerY), 2));
        }
        else 
        {
            y++;
            x = centerX - sqrt(pow(r,2) - pow((y-centerY), 2));
        }
        point.x = x;
        point.y = y;
        p.emplace_back(point);

        if(y == centerY-r || y == centerY || y == centerY+r) part++;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderDrawLines(renderer, p.data(), p.size());
}

void draw()
{
    // Set the render draw color (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 10,10,10, 255);

    // Clear the renderer with the specified draw color
    SDL_RenderClear(renderer);


    newRadius+= .2;
    if((newRadius*2) >= SCREEN_HEIGHT || (newRadius*2) >= SCREEN_WIDTH)
        newRadius = radius;
    drawCircle(newRadius);
    SDL_RenderPresent(renderer);

    // Present the renderer (draw the frame to the window)
    
}

void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char **argv)
{
    // The game loop control variable
    gameIsRunning = initializeWindow();

    startTime = SDL_GetTicks();

    // Game loop: keep the application running until 'running' is set to false
    while (gameIsRunning)
    {
        // Continuously polls for SDL events
        process_input();

        // Draw the rendered window
        draw();
    }

    // Clean up and exit the application
    destroyWindow();

    return 0;
}
