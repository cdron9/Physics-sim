#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <math.h>

#define TARGET_FPS 120
#define FRAME_TIME (1000.0f / TARGET_FPS)
#define GRAVITY 9.8f
#define FRICTION 0.999f // slowly decrease velocity
#define RESTITUTION 0.9f // to calculate how much to bounce

typedef struct {
    float centerX, centerY;     // position, center of ball
    float vx, vy;               // velocity in pixels/second
    float radius;
    SDL_Color color;
} Ball;

typedef struct{
    float centerX, centerY;
    float radius;
} Container;

void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius);
void DrawCircle(SDL_Renderer *renderer, int cx, int cy, int radius);

void grav_updateBallPhys(Ball *ball, float deltaTime);
void grav_updateBallPos(Ball *ball, float deltaTime);

void collision_andAction(Ball *ball, Container container);


int main(int argc, char* argv[]) {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    bool quit = false;
    SDL_Event e;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldnt initialise SDL: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("circles window", 1280,960,0); // 0 for no flags
    renderer = SDL_CreateRenderer(window,NULL); //null let adl autopick rendering driver

    if (window == NULL || renderer == NULL) {
        SDL_Log("Failed to open window or renderer: %s\n", SDL_GetError());
        return 1;
    }

    Container container = {640.0f, 480.0f, 300.0f};
    Ball ball = {640.0f, 300.0f, 600.0f, 0.0f, 20.0f, {255, 255, 255, 255}};
    
    Uint64 lastTime = SDL_GetTicks();

    while (!quit) {

       Uint64 currentTime = SDL_GetTicks();
       float deltaTime = (currentTime - lastTime) / 1000.0f; // converted to seconds
       lastTime = currentTime;
    // TODO: use deltatime for physics.

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        DrawCircle(renderer, container.centerX, container.centerY, container.radius);

        SDL_SetRenderDrawColor(renderer, ball.color.r, ball.color.g, ball.color.b, ball.color.a);
        DrawFilledCircle(renderer, ball.centerX, ball.centerY, ball.radius);

        grav_updateBallPhys(&ball, deltaTime);
        grav_updateBallPos(&ball, deltaTime);
        
        collision_andAction(&ball, container);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

 void DrawCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = radius;
    int y = 0;
    int decision = 1 - x;

    while (y<= x) {
        SDL_RenderPoint(renderer,  cx + x, cy + y);
        SDL_RenderPoint(renderer,  cx + y, cy + x);
        SDL_RenderPoint(renderer,  cx - y, cy + x);
        SDL_RenderPoint(renderer,  cx - x, cy + y);
        SDL_RenderPoint(renderer,  cx - x, cy - y);
        SDL_RenderPoint(renderer,  cx - y, cy - x);
        SDL_RenderPoint(renderer,  cx + y, cy - x);
        SDL_RenderPoint(renderer,  cx + x, cy - y);

        y++;

        if (decision <= 0)
            decision += 2 * y + 1;
        else {
            x--;
            decision += 2 * (y-x) + 1;
        }
    }
}

void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = radius;
    int y = 0;
    int decision = 1 - x;

    while (y <= x ) {
        SDL_RenderLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderLine(renderer, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderLine(renderer, cx - y, cy + x, cx + y, cy + x);
        SDL_RenderLine(renderer, cx - y, cy - x, cx + y, cy - x);

        y++;

        if (decision <= 0)
            decision += 2 * y + 1;
        else {
            x--;
            decision += 2 * (y-x) + 1;
        }
    }
}

void grav_updateBallPhys(Ball *ball, float deltaTime) {
    ball->vx *= FRICTION;
    ball->vy += GRAVITY * 100 * deltaTime;
}

void grav_updateBallPos(Ball *ball, float deltaTime) {
    ball->centerX += ball->vx * deltaTime;
    ball->centerY += ball->vy * deltaTime;
}
    
void collision_andAction(Ball *ball, Container container) {
    float x_dist = ball->centerX - container.centerX;  // Vector from container center to ball
    float y_dist = ball->centerY - container.centerY;

    float distance = sqrt((x_dist * x_dist) + (y_dist * y_dist));

    // Avoid division by zero if ball is exactly at center
    if (distance < 0.001f) {
        distance = 0.001f;
    }

    if (distance + ball->radius >= container.radius) {
        // calculate the normal
        float nx = -x_dist / distance;  // negative to reflect off inside of cont
        float ny = -y_dist / distance;
        
        // calculate dot product of velocity & normal
        float dot = ball->vx * nx + ball->vy * ny;
        
        // only reflect if moving towards the wall (dot < 0 since normal points inward)
        if (dot < 0) {
            ball->vx = ball->vx - 2 * dot * nx;
            ball->vy = ball->vy - 2 * dot * ny;
            
            // apply energy loss (bounciness)
            ball->vx *= RESTITUTION;
            ball->vy *= RESTITUTION;
        }
        // stop ball getting stuck in collision
        float overlap = (distance + ball->radius) - container.radius;
        ball->centerX += nx * overlap;  
        ball->centerY += ny * overlap;
    }
}