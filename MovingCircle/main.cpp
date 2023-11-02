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
#define SCREEN_HEIGHT 520
#define NUM_PARTICLES 10000 // particles to create visual effect

// Global variables
bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Uint32 startTime;
Uint32 currentTime;
Uint32 elapsedTime;
const int r = 80;
double centerX = -r;
const int centerY = (SCREEN_HEIGHT / 2);
double controlX = (SCREEN_WIDTH / 2);
double controlY = 0.0;
int flag = 0, centerOfCollision = 0;

// implimanting visual effect(particle effect)
typedef struct
{
    float x, y;    // Position
    float angle;   // Angle for swirling motion
    int lifetime;  // How long the particle will exist
    Uint8 r, g, b; // Color
} Particle;

bool initializeWindow(void);

void initializeParticles(Particle particles[]);
void visualEffect(Particle particles[]); // basic wind effect(2d simulation of a tornado)

void drawCircle(int X, int Y);
void process_input(void);
void collisionDetection(void);
void movingCircle(void);
void draw(Particle particles[]);

void destroyWindow(void);

int main(int argc, char **argv)
{
    // The game loop control variable
    gameIsRunning = initializeWindow();
    startTime = SDL_GetTicks();

    // initialize particles for visual effect
    Particle particles[NUM_PARTICLES];

    while (gameIsRunning)
    {
        process_input();
        draw(particles);
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

void initializeParticles(Particle particles[])
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles[i].x = centerX + (controlX - centerX) / 2; // Start at the center of collision
        particles[i].y = centerY + (controlY - centerY) / 2;
        particles[i].angle = (float)(rand() % 360) * M_PI / 180.0; // Random initial angle
        particles[i].lifetime = 200 + (rand() % 100);              // Random lifetime
        particles[i].r = 0;
        particles[i].g = 255;
        particles[i].b = 255;
    }
}

void visualEffect(Particle particles[]) // basic wind effect(2d simulation of a tornado)
{
    // Update particles
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles[i].x += cos(particles[i].angle) * 2; // Move based on angle
        particles[i].y += sin(particles[i].angle) * 2;
        particles[i].angle += 0.02; // Increase angle for swirling motion
        particles[i].lifetime--;

        if (particles[i].lifetime <= 0)
        {
            particles[i].x = centerX + (controlX - centerX) / 2; // reset to center
            particles[i].y = centerY + (controlY - centerY) / 2;
            particles[i].angle = (float)(rand() % 360) * M_PI / 180.0;
            particles[i].lifetime = 200 + (rand() % 100);
        }
    }

    // Render particles
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        SDL_SetRenderDrawColor(renderer, particles[i].r, particles[i].g, particles[i].b, 255);
        SDL_RenderDrawPoint(renderer, particles[i].x, particles[i].y);
    }
    SDL_Delay(8);
}

void drawCircle(int X, int Y)
{
    double angle = 0.0 * (M_PI / 180); // convert the angle into radian

    vector<SDL_Point> points;
    while (angle <= (360 * (M_PI / 180)))
    {
        SDL_Point point;
        point.x = X + r * cos(angle);
        point.y = Y - r * sin(angle);
        points.push_back(point);

        angle += 6 * (M_PI / 180);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderDrawLines(renderer, points.data(), points.size());
}

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameIsRunning = false; // Exit the game loop
            break;
        case SDL_KEYDOWN:
            if (!flag)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    drawCircle(controlX, controlY -= 5);
                    break;
                case SDLK_DOWN:
                    drawCircle(controlX, controlY += 5);
                    break;
                case SDLK_LEFT:
                    drawCircle(controlX -= 5, controlY);
                    break;
                case SDLK_RIGHT:
                    drawCircle(controlX += 5, controlY);
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
}

void collisionDetection(void)
{
    // calculate the distance between the center of both circles
    double centerDistance = sqrt(pow(centerX - controlX, 2) + pow(centerY - controlY, 2));
    if (centerDistance <= 2 * r && !flag) // collision occured;
    {
        SDL_Log("Collision Detected!\n");
        // stop movement for both of the circles
        flag = 1;
    }
}

void movingCircle(void)
{
    drawCircle(centerX, centerY);

    currentTime = SDL_GetTicks();
    elapsedTime = (currentTime - startTime); // calculate time in milliseconds

    // change the center of the moving circle by 1 after every 2 milliseconds
    if (elapsedTime % 2 == 0)
    {
        centerX += 1;
        if ((centerX - r) > SCREEN_WIDTH)
            centerX = -r;
    }
}

void draw(Particle particles[])
{
    // set renderer draw color(R,G,B,A) & clear the renderer with that color
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);

    collisionDetection(); // detect if there is any collision

    if (!flag) // No Collision
    {
        // Draw a circle
        movingCircle();
    }
    else // collision detected
    {
        if (!centerOfCollision)
        {
            initializeParticles(particles);
            centerOfCollision = 1;
        }
        visualEffect(particles);
        drawCircle(centerX, centerY); // moving circle cannot move anymore
    }

    // controlable circle
    if (controlX > SCREEN_WIDTH || controlX < 0 || controlY < 0 || controlY > SCREEN_HEIGHT)
    {
        // reset the controlable circle to its initial position
        controlX = (SCREEN_WIDTH / 2);
        controlY = 0;
    }
    drawCircle(controlX, controlY);

    // present the renderer(e.g draw the frame to the window)
    SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}