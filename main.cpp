#include<vector>
#include<iostream>
#include<SDL2/SDL.h>
#include "constants.h"

#undef main

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };



struct Snake {
    Direction direction;
    vector<SDL_Rect> segments;
};


float last_frame_time=0;


void initialize_snake(Snake& snake) {
    snake.direction = RIGHT;
    SDL_Rect initial_segment = {WIDTH / 2, HEIGHT / 2, 20, 20};
    snake.segments.push_back(initial_segment);
}
//SDL_Rect moving_rect{WIDTH/2,HEIGHT/2,20,20};

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



void grow_snake(Snake& snake) {
    SDL_Rect new_segment = snake.segments.back();
    snake.segments.push_back(new_segment);
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

void move_snake(Snake& snake) {
   

    for (int i = snake.segments.size() - 1; i > 0; --i) {
        snake.segments[i] = snake.segments[i - 1];
    }


    SDL_Rect& head = snake.segments[0];

    switch (snake.direction) {    
        case UP:
            if(head.y<=-10) head.y= HEIGHT-10;
            else
                head.y -= 10;
            break;
        case DOWN:
            if (head.y >= HEIGHT -10)  head.y=-10;
            else
                head.y += 10;
            break;
        case LEFT:
            if(head.x<= -10) head.x = WIDTH -10;
            else
                head.x -= 10;
            break;
        case RIGHT:
            if(head.x>= WIDTH-10) head.x = -10;
            else
                head.x += 10;
            break;
    }

    
}

void render(const Snake& snake) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(int i= 0; i<snake.segments.size();i++){
        SDL_RenderFillRect(renderer, &snake.segments[i]);
    }    
}


void destroy_window() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


int main () {

    game_is_running = initialize_window();
    Snake snake;
    initialize_snake(snake);

    Food food;
    initialize_food(food);

    while(game_is_running){
        process_input(snake);
        move_snake(snake);

        if(collision(food.rect,snake.segments[0])){
            initialize_food(food);
            grow_snake(snake);
        }    

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        render(snake);

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&food.rect);
        

        SDL_RenderPresent(renderer);
        SDL_Delay(33);

    }
    destroy_window();

}