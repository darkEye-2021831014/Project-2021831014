#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 520
#define NUM_PARTICLES 10000

// Global variables
bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int redColorCode = 0;
Uint32 startTime;
Uint32 currentTime;
const int r = 80;
double centerX = -r;
const int centerY = (SCREEN_HEIGHT/2);
double controlX = (SCREEN_WIDTH/2);
double controlY = 0.0;
int flag = 0, ones = 0;

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




//implimanting visual effect(particle effect)
typedef struct {
    float x, y;     // Position
    float angle;    // Angle for swirling motion
    int lifetime;   // How long the particle will exist
    Uint8 r, g, b;  // Color
} Particle;

void initializeParticles(Particle particles[]) {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x = centerX + (controlX-centerX) / 2;  // Start at the center
        particles[i].y = centerY + (controlY-centerY)/2;
        particles[i].angle = (float)(rand() % 360) * M_PI / 180.0; // Random initial angle
        particles[i].lifetime = 200 + (rand() % 100);  // Random lifetime
        particles[i].r = 0;
        particles[i].g = 255;
        particles[i].b = 255;  // Blue color
    }
}

void visualEffect(Particle particles[])   //basic wind effect(2d simulation of a tornado)
{
    // Update particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x += cos(particles[i].angle) * 2;  // Move based on angle
        particles[i].y += sin(particles[i].angle) * 2;
        particles[i].angle += 0.02;  // Increase angle for swirling motion
        particles[i].lifetime--;

        if (particles[i].lifetime <= 0) {
            particles[i].x = centerX + (controlX-centerX) / 2;  //reset to center
            particles[i].y = centerY + (controlY-centerY)/2;
            particles[i].angle = (float)(rand() % 360) * M_PI / 180.0;
            particles[i].lifetime = 200 + (rand() % 100);
        }
    }


    // Render particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        SDL_SetRenderDrawColor(renderer, particles[i].r, particles[i].g, particles[i].b, 255);
        SDL_RenderDrawPoint(renderer, particles[i].x, particles[i].y);
    }
    SDL_Delay(8);
}












void drawCircle(int X, int Y)   
{
    vector<SDL_Point> p;
    
    SDL_Point point;
    int part = 0, y = Y, x = 0;
    while(part != 4)
    {
        if(part == 0 || part == 3) 
        {
            y--;
            x = X + sqrt(pow(r,2) - pow((y-Y), 2));
        }
        else 
        {
            y++;
            x = X - sqrt(pow(r,2) - pow((y-Y), 2));
        }
        point.x = x;
        point.y = y;
        p.emplace_back(point);

        if(y == Y-r || y == Y || y == Y+r) part++;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderDrawLines(renderer, p.data(), p.size());

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
        case SDL_KEYDOWN:
            if(!flag)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        drawCircle(controlX, controlY-=3);
                        break;
                    case SDLK_DOWN:
                        drawCircle(controlX, controlY+=3);
                        break;
                    case SDLK_LEFT:
                        drawCircle(controlX-=3, controlY);
                        break;
                    case SDLK_RIGHT:
                        drawCircle(controlX+=3, controlY);
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
    //calculate the distance between the center of both circles
    double centerDistance = sqrt(pow(centerX-controlX, 2)+pow(centerY-controlY,2));
    if(centerDistance <= 2*r) //collision occured;
    {
        // cout << "Collision Detected!\n";
        // pause the screen 
        flag = 1;
        //set to initial position
        // centerX = -r;
        // controlX = (SCREEN_WIDTH/2);
        // controlY = 0.0;
    }
}



void movingCircle(void)
{
    drawCircle(centerX, centerY);

    centerX+=0.1;
    if((centerX-r) > SCREEN_WIDTH) centerX = -r;  
}



void draw(Particle particles[])
{
    // Set the render draw color (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 10,10,10, 255);

    // Clear the renderer with the specified draw color
    SDL_RenderClear(renderer);

    collisionDetection();

    if(!flag)  //No Collision
    {
        //Draw a circle
        movingCircle();
    }
    else //collision detected
    {
        if(!ones) 
        {
            initializeParticles(particles);
            ones = 1;
        }
        visualEffect(particles);
        drawCircle(centerX,centerY);
    }

    //controlable circle
    if(controlX > SCREEN_WIDTH || controlX < 0 || controlY < 0 || controlY > SCREEN_HEIGHT) 
    {
        controlX = (SCREEN_WIDTH/2);
        controlY = 0;
    }
    drawCircle(controlX, controlY);
    

    // Present the renderer (draw the frame to the window)
    SDL_RenderPresent(renderer);
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

    //initialize particles for visual effect
    Particle particles[NUM_PARTICLES];
    

    // Game loop: keep the application running until 'running' is set to false
    while (gameIsRunning)
    {
        // Continuously polls for SDL events
        process_input();

        // Draw the rendered window
        draw(particles);
    }

    // Clean up and exit the application
    destroyWindow();

    return 0;
}
