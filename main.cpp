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


void grow_snake(Snake& snake) {
    SDL_Rect new_segment = snake.segments.back();
    snake.segments.push_back(new_segment);
}

void initialize_snake(Snake& snake) {
    snake.direction = RIGHT;
    SDL_Rect initial_segment = {WIDTH / 2, HEIGHT / 2, 20, 20};

    int i= 5;

    while(i--){
        snake.segments.push_back(initial_segment);
    }  
}


int game_is_running = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int last_time= 0;




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






int collision(const SDL_Rect &a , const SDL_Rect &b) {
    return SDL_HasIntersection(&a,&b);
}


bool food_on_snake(const Food& food, const Snake& snake) {
    for (const auto& segment : snake.segments) {
        if (collision(food.rect, segment)) {
            return true;
        }
    }
    return false;
}



void initialize_food(Food& food, const Snake& snake) {
    do {
        food.rect.w = 20;
        food.rect.h = 20;
        food.rect.x = rand() % (WIDTH - food.rect.w);
        food.rect.y = rand() % (HEIGHT - food.rect.h);
    } while (food_on_snake(food, snake));
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
            if (head.y >= HEIGHT-10)  head.y= -10;
            else
                head.y += 10;
            break;
        case LEFT:
            if(head.x<=-10) head.x = WIDTH-10;
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

int hit_itself(Snake &snake){
     for(int i= 4; i<snake.segments.size()-1;i++){
        if(collision(snake.segments[0],snake.segments[i])){
            return 1;
        }
     }
     return 0;
}

void eyes(const Snake&snake) {
    SDL_Rect eye1;
    SDL_Rect eye2;
    switch (snake.direction)
    {
    case RIGHT:
            eye1.w=5;
            eye1.h=5;
            eye1.x=snake.segments[0].x+10;
            eye1.y=snake.segments[0].y+2;

            eye2 = eye1;

            eye2.y =eye1.y+11;
        
        break;

    case LEFT:
            eye1.w=5;
            eye1.h=5;
            eye1.x=snake.segments[0].x+5;
            eye1.y=snake.segments[0].y+2;

            eye2 = eye1;

            eye2.y =eye1.y+11;
        
        break;

    case UP:
            eye1.w=5;
            eye1.h=5;
            eye1.x=snake.segments[0].x+2;
            eye1.y=snake.segments[0].y+5;

            eye2 = eye1;

            eye2.x =eye1.x+11;
        
        break;  

    case DOWN:
            eye1.w=5;
            eye1.h=5;
            eye1.x=snake.segments[0].x+2;
            eye1.y=snake.segments[0].y+10;

            eye2 = eye1;

            eye2.x =eye1.x+11;
        
        break;
    }
    SDL_RenderFillRect(renderer,&eye1);
    SDL_RenderFillRect(renderer,&eye2);

}

void render(const Snake& snake) {
    int color = 255;

   

    for(int i= 0; i<snake.segments.size();i++){
        SDL_SetRenderDrawColor(renderer, 0, color, 0, 255);
        SDL_RenderFillRect(renderer, &snake.segments[i]);
        if(color>=200)   color-=3;
        else if (color>70) color-=1;
        else color = 70;
    }   
    
    //eyes(snake);
}

void eye_animation(const Snake &snake, int ticks) {
    int time ;

    if(ticks<10000) time = SDL_GetTicks();
    else
        time= SDL_GetTicks() % 10000;

    if((time>4000 && time<4300)||(time>9500 && time <9650)||(time>9750 && time <9900)){
        SDL_SetRenderDrawColor(renderer,0,210,0,255);
        eyes(snake);
    }    
    else{
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        eyes(snake);
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
    initialize_food(food,snake);

    

    while(game_is_running){
        process_input(snake);
        move_snake(snake);

       
        if(collision(food.rect,snake.segments[0])){
            initialize_food(food,snake);
            grow_snake(snake);
        }   

        

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        if(hit_itself(snake)){
            game_is_running=0;
        }


        render(snake);

        int ticks = SDL_GetTicks();
        eye_animation(snake,ticks);

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&food.rect);
        

        SDL_RenderPresent(renderer);
        SDL_Delay(33);

    }
    destroy_window();

}