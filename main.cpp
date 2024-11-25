#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include "constants.h"
#include<SDL2/SDL_ttf.h>

#undef main

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

int gameover=0;
int score = 0;
float speed =7;

struct Snake {
    Direction direction;
    vector<SDL_Rect> segments;
};
struct Wall{
    vector<SDL_Rect> walls;
};

void grow_snake(Snake& snake) {
    int k= 2;
    while(k--){
        SDL_Rect new_segment = snake.segments.back();
        snake.segments.push_back(new_segment);
    }
    score+=10;
}

void initialize_snake(Snake& snake) {
    snake.direction = RIGHT;
    SDL_Rect initial_segment = {WIDTH / 2, HEIGHT / 2, 20, 20};

    int i= 3;

    while(i--){
        snake.segments.push_back(initial_segment);
    }  
}


int game_is_running = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
//int last_time= 0;
TTF_Font* font = NULL;

int initialize_window (){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) {
        fprintf(stderr,"error initializing the sdl");
        return 0;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr,"error initializing the ttf");
        return 0;
    }
    
    window = SDL_CreateWindow("Snake game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_BORDERLESS);
    if(!window) {
        fprintf(stderr,"error initializing window");
        return 0;
    }

    renderer =SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer){
        fprintf(stderr,"error initializing the renderer");
        return 0;
    }

    return 1;

}

struct Food {
    SDL_Rect rect;
    SDL_Rect outerlayer;
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

        food.outerlayer = food.rect;

        food.outerlayer.w += 50;
        food.outerlayer.h += 50;
        food.outerlayer.x -= 25;
        food.outerlayer.y-=25;
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

void restart_game(Snake& snake,Food& food) {
    initialize_snake(snake);
    initialize_food(food, snake);
    score =0;
    speed =7;
    
}
int iscr =0;
bool flag=0;
void move_snake(Snake& snake,int score) {
   
    if(speed>=12) flag==1;
    
    if(iscr!=score && flag==0 ) {
        speed+= speed*0.0075; 
        iscr =score;
        
    }
   
    for (int i = snake.segments.size() - 1; i > 0; --i) {
        snake.segments[i] = snake.segments[i - 1];
    }


    SDL_Rect& head = snake.segments[0];

    switch (snake.direction) {    
        case UP:
            if(head.y<=-10) head.y= HEIGHT-10;
            else
                head.y -= speed;
            break;
        case DOWN:
            if (head.y >= HEIGHT-10)  head.y= -10;
            else
                head.y += speed;
            break;
        case LEFT:
            if(head.x<= -10) head.x = WIDTH-10;
            else
                head.x -= speed;
            break;
        case RIGHT:
            if(head.x>= WIDTH-10) head.x = -10;
            else
                head.x += speed;
            break;
    }

    
}

int hit_itself(Snake &snake){
     for(int i= 5; i<snake.segments.size();i++){
        if(collision(snake.segments[0],snake.segments[i])){
            return 1;
        }
     }
     return 0;
}

void eyes(const Snake&snake,int ticks,bool over) {

    SDL_Rect eye1;
    SDL_Rect eye2;
    SDL_Rect eyew1;
    SDL_Rect eyew2;
    
    switch (snake.direction)
    {
        case RIGHT:
            eye1.w=8;
            eye1.h=6;
            eyew1.w =11;
            eyew1.h=7;

            eye1.x=snake.segments[0].x+8;
            eye1.y=snake.segments[0].y+2;
            eyew1.x=eye1.x-2; 
            eyew1.y=eye1.y-1;

            eye2 = eye1;
            eyew2 = eyew1;

            eyew2.y =eyew1.y+10;
            eye2.y =eye1.y+10;
            
        
            break;

        case LEFT:
            eye1.w=8;
            eye1.h=6;
            eyew1.w = 11;
            eyew1.h=7;

            eye1.x=snake.segments[0].x+4;
            eye1.y=snake.segments[0].y+2;
            eyew1.x=eye1.x-1; 
            eyew1.y=eye1.y-1;


            eye2 = eye1;
            eyew2 = eyew1;

            eyew2.y =eyew1.y+10;
            eye2.y =eye1.y+10;
        
            break;

        case UP:
            eye1.w=6;
            eye1.h=8;
            eyew1.w=7;
            eyew1.h=11;

            eye1.x=snake.segments[0].x+2;
            eye1.y=snake.segments[0].y+4;

            eyew1.x=eye1.x-1;
            eyew1.y=eye1.y-1;

            eye2 = eye1;
            eyew2=eyew1;

            eyew2.x=eye1.x+10;
            eye2.x =eye1.x+10;
        
            break;  

        case DOWN:
           /* eye1.w=5;
            eye1.h=5;
            eye1.x=snake.segments[0].x+2;
            eye1.y=snake.segments[0].y+10;

            eye2 = eye1;

            eye2.x =eye1.x+11;
        */  eye1.w=6;
            eye1.h=8;
            eyew1.w=7;
            eyew1.h=11;

            eye1.x=snake.segments[0].x+2;
            eye1.y=snake.segments[0].y+8;

            eyew1.x=eye1.x-1;
            eyew1.y=eye1.y-2;

            eye2 = eye1;
            eyew2=eyew1;

            eyew2.x=eye1.x+10;
            eye2.x =eye1.x+10;

            break;
    }
 
   
    if(!over){
         int time = ticks % 10000;

        bool isEyeClosed = (time > 4000 && time < 4300) || (time > 9300 && time < 9500) || (time > 9750 && time < 9999);

        if(isEyeClosed){
            SDL_SetRenderDrawColor(renderer, 0, 190, 0, 255);

    
            SDL_RenderFillRect(renderer,&eyew1);
            SDL_RenderFillRect(renderer,&eyew2);
    
            SDL_RenderFillRect(renderer,&eye1);
            SDL_RenderFillRect(renderer,&eye2);

        }
        else{
            SDL_SetRenderDrawColor(renderer, 240, 240, 255, 255);
            SDL_RenderFillRect(renderer,&eyew1);
            SDL_RenderFillRect(renderer,&eyew2);
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer,&eye1);
            SDL_RenderFillRect(renderer,&eye2);
        }
    
    }
   
    else{
        SDL_SetRenderDrawColor(renderer, 0,190,0,255);
        SDL_RenderFillRect(renderer,&eyew1);
        SDL_RenderFillRect(renderer,&eyew2);
    }
    
   
}
void initialize_walls( Wall &wall){

    
    SDL_Rect wall1={0,50,5,HEIGHT-100};
    
    SDL_Rect wall2=wall1;
    wall2.x=WIDTH-5;
    wall.walls.push_back(wall1);
    wall.walls.push_back(wall2);
   
}

void render(const Snake& snake,const Wall& wall){ 
    SDL_SetRenderDrawColor(renderer, 180, 180, 180,1);
    SDL_RenderFillRect(renderer,&wall.walls[0]);
    SDL_RenderFillRect(renderer,&wall.walls[1]);
    int color = 255;

   

    for(int i= 0; i<snake.segments.size();i++){
        SDL_SetRenderDrawColor(renderer, 0, color, 0, 255);
        SDL_RenderFillRect(renderer, &snake.segments[i]);
        if(color>=200)   color-=3;
        else if (color>70) color-=1;
        else color = 70;
        
    }   

    
}





void render_score(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = {255, 255, 255}; 
    string scoreText = "Score : " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 10; 
    textRect.y = 10;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}



void tongue(const Snake& snake) {
    SDL_Rect tongue;
    int k=3;//num of tongue breaks
    for(int i =0 ;i<k;i++){
        
    
        switch (snake.direction) {
            case UP:
                tongue.x = snake.segments[0].x+7+rand()%(6); //here 6 came from (width-2*twidth)/2
                tongue.y = snake.segments[0].y-tlen-(i*tlen);
                tongue.w = 4;
                tongue.h = tlen;
                break;
            case DOWN:
                tongue.x = snake.segments[0].x + 7+rand()%6;
                tongue.y = snake.segments[0].y +20+(i*tlen);
                tongue.w = 4;
                tongue.h = tlen;
                break;
            case LEFT:
                tongue.x = snake.segments[0].x -tlen-(tlen*i);
                tongue.y = snake.segments[0].y + 7+rand()%6;
                tongue.w = tlen;
                tongue.h = 4;
                break;
            case RIGHT:
                tongue.x = snake.segments[0].x +20+(tlen*i);
                tongue.y = snake.segments[0].y + 7+rand()%6;
                tongue.w = tlen;
                tongue.h = 4;
                break;
        }
        
        SDL_RenderFillRect(renderer, &tongue);
    }
}

void animateTongue(const Snake& snake, int ticks) {
    int time = ticks % 13000;
    bool isTongueout = (time >4000  && time < 4600) || (time>11000 && time < 12000);

   /* SDL_Color tongueColor = (isTongueout ? (SDL_Color){120, 0, 0, 255} : (SDL_Color){0, 0, 0, 255});
    SDL_SetRenderDrawColor(renderer, tongueColor.r, tongueColor.g, tongueColor.b, tongueColor.a);
*/  
    if(isTongueout){
        SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255);
        tongue(snake);
    }
   
}
void mouthOpen (const Snake& snake){
    SDL_Rect mouth;
    SDL_Rect lips;
    SDL_Rect t1;
    SDL_Rect t2;

    switch (snake.direction){
        case RIGHT:
            mouth.w=15;
            mouth.h=25;
            mouth.x=snake.segments[0].x+20-1;
            mouth.y=snake.segments[0].y-2.5;
            lips=mouth;
            lips.w+=3;
            lips.h+=3;
            lips.x-=1.5;
            lips.y-=1.5;
            t1=mouth;
            t1.w=7;
            t1.h=4;
            t1.y+=3;
            t2=t1;
            t2.y+=15;
            break;

        case LEFT:
            mouth.w=15;
            mouth.h=25;
            mouth.x=snake.segments[0].x-15+1;
            mouth.y=snake.segments[0].y-2.5;
            lips=mouth;
            lips.w+=3;
            lips.h+=3;
            lips.x-=1.5;
            lips.y-=1.5;
            t1=mouth;
            t1.w=7;
            t1.h=4;
            t1.y+=3;
            t1.x+=10;
            t2=t1;
            t2.y+=15;
        
             break;

        case UP:
            mouth.w=25;
            mouth.h=15;
            mouth.x=snake.segments[0].x-2.5;
            mouth.y=snake.segments[0].y-15+1;
            lips=mouth;
            lips.w+=3;
            lips.h+=3;
            lips.x-=1.5;
            lips.y-=1.5;
            t1=mouth;
            t1.w=4;
            t1.h=7;
            t1.y+=10;
            t1.x+=3;
            t2=t1;
            t2.x+=15;
             break;  

        case DOWN:
            mouth.w=25;
            mouth.h=15;
            mouth.x=snake.segments[0].x-2.5;
            mouth.y=snake.segments[0].y+20-1;
            lips=mouth;
            lips.w+=3;
            lips.h+=3;
            lips.x-=1.5;
            lips.y-=1.5;
            t1=mouth;
            t1.w=4;
            t1.h=7;
            t1.x+=3;
            t2=t1;
            t2.x+=15;

            break;
    }    
    SDL_SetRenderDrawColor(renderer, 0,190, 0, 255);
    SDL_RenderFillRect(renderer, &lips);

    SDL_SetRenderDrawColor(renderer, 90, 0, 0, 255);
    SDL_RenderFillRect(renderer, &mouth);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &t1);
    SDL_RenderFillRect(renderer, &t2);

    
}
void animateMouth(const Snake& snake, SDL_Rect food_smell) {
    if(collision(snake.segments[0],food_smell)){
        mouthOpen(snake);
    }
    
}



void destroy_window() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}


int main () {

    game_is_running = initialize_window();

    Wall wall;
    initialize_walls(wall);
    
    Snake snake;
    initialize_snake(snake);

    

    Food food;
    initialize_food(food,snake);

    font = TTF_OpenFont("extensions/PixeloidSansBold-PKnYD.ttf", 18);
    
    if (!font) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        return 0;
    }

    

    while(game_is_running){
        
        int ticks = SDL_GetTicks();
        process_input(snake);
        if(gameover){
            SDL_Event event;
            SDL_PollEvent(&event);
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r){
                snake.segments.erase(snake.segments.begin(),snake.segments.end());
               
                gameover=0;
                
                restart_game(snake,food);
            }
        }
       
        else if(gameover==0){
            move_snake(snake,score);

        
           
            if(collision(food.rect,snake.segments[0])){
                initialize_food(food,snake);     

                grow_snake(snake);     
              
            }   
    
       
       

            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
        
        

            if(hit_itself(snake)|| collision(snake.segments[0],wall.walls[0])|| collision(snake.segments[0],wall.walls[1])){
                gameover=1;
                //game_is_running=0;
            }


            render(snake,wall);

            animateTongue(snake,ticks);
            eyes(snake,ticks,gameover);
            animateMouth(snake,food.outerlayer);
        
            //food_in_belly(snake,prevfood);

            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderFillRect(renderer,&food.rect);
            render_score(renderer, font, score);
          

            SDL_RenderPresent(renderer);
            SDL_Delay(33);
        } 
        
    }
    destroy_window();

}