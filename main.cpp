#include<iostream>
#include<SDL2/SDL.h>
#include "constants.h"
#include<vector>
#undef main

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };



struct Snake {
    Direction direction;
};


float last_frame_time=0;

SDL_Rect moving_rect{WIDTH/2,HEIGHT/2,20,20};

int game_is_running = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int initialize_window (){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) {
        fprintf(stderr,"error initializing the sdl");
        return 0;
    }

    window = SDL_CreateWindow("SNake game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_BORDERLESS);
    if(!window) {
        fprintf(stderr,"error initializing window");
        return 0;
    }

    renderer =SDL_CreateRenderer(window,-1,0);
    if(!renderer){
        fprintf(stderr,"error initializing the renderer");
        return 0;
    }

    return 1;

}

struct Food {
    SDL_Rect rect;
};


void initialize_food(Food & food){
    food.rect.h = 20;
    food.rect.w = 20;
    food.rect.x = rand() % (WIDTH- food.rect.w);
    food.rect.y = rand() % (HEIGHT - food.rect.h);
}

int collision(const SDL_Rect &a , const SDL_Rect &b) {
    return SDL_HasIntersection(&a,&b);
}


void process_input(Snake& snake) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (snake.direction != DOWN) snake.direction = UP;
                    break;
                case SDLK_DOWN:
                    if (snake.direction != UP) snake.direction = DOWN;
                    break;
                case SDLK_LEFT:
                    if (snake.direction != RIGHT) snake.direction = LEFT;
                    break;
                case SDLK_RIGHT:
                    if (snake.direction != LEFT) snake.direction = RIGHT;
                    break;
                case SDLK_ESCAPE:
                    game_is_running = 0;
                    break;
            }
            break;
        case SDL_QUIT:
            game_is_running = 0;
            break;
    }
}

void move_snake(const Snake& snake, SDL_Rect& rect) {
    switch (snake.direction) {
        case UP:
            if(rect.y<=-10) rect.y= HEIGHT-10;
            else
                rect.y -= 10;
            break;
        case DOWN:
            if (rect.y >= HEIGHT -10)  rect.y=-10;
            else
                rect.y += 10;
            break;
        case LEFT:
            if(rect.x<= -10) rect.x = WIDTH -10;
            else
                rect.x -= 10;
            break;
        case RIGHT:
            if(rect.x>= WIDTH-10) rect.x = -10;
            else
                rect.x += 10;
            break;
    }

    
}

void render(const Snake& snake, SDL_Rect& rect) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}


void destroy_window() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


int main () {

    game_is_running = initialize_window();
    Snake snake;
    snake.direction =RIGHT;
    Food food;
    initialize_food(food);

    while(game_is_running){
        process_input(snake);
        move_snake(snake,moving_rect);

        if(collision(food.rect,moving_rect))
            initialize_food(food);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        render(snake,moving_rect);

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&food.rect);
        

        SDL_RenderPresent(renderer);
        SDL_Delay(33);

    }
    destroy_window();

}