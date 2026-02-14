#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "entity.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

SDL_Color fColor;
SDL_Color fBlack;
SDL_Rect fontRect;
TTF_Font* font;
TTF_Font* fontSTART;

void fontInit(){
        TTF_Init();
        font = TTF_OpenFont("font/retro.ttf", 26);
        fontSTART = TTF_OpenFont("font/retro.ttf", 45);
        fColor.r = 255;
        fColor.g = 255;
        fColor.b = 255;
        fBlack.r = 0;
        fBlack.g = 0;
        fBlack.b = 0;
}

void display_text(TTF_Font* font, SDL_Color color, int x, int y, char text[], SDL_Renderer *renderer){
    SDL_Surface* fontSurface = TTF_RenderText_Solid(font, text, color);
            if (fontSurface) {
                SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
                SDL_Rect fontRect = {x, y, fontSurface->w, fontSurface->h};
                SDL_RenderCopy(renderer, fontTexture, NULL, &fontRect);
                SDL_DestroyTexture(fontTexture);
                SDL_FreeSurface(fontSurface);
            }
}

//rendu graphique
bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool enemy_bullet_active, Gamestate gamestate, Entity_bullet heart, bool heart_active, Menustate *menustate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gamestate==0){
        SDL_Rect startmenu_rect = {
                        100, 50,
                        600, 500};
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderFillRect(renderer, &startmenu_rect);
        
        display_text(fontSTART, fColor, 315, 55, "Menu", renderer);
        
        display_text(fontSTART, fColor, 170, 150, "1)      NEW GAME", renderer);

        display_text(fontSTART, fColor, 170, 250, "2)      LOAD GAME", renderer);

        display_text(fontSTART, fColor, 170, 350, "3)      QUIT", renderer);
        
        SDL_Rect menu_case = {
                        150, 150 + (*menustate) * 100,
                        500, 50};
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &menu_case);

        switch (*menustate)
        {
        case NEW_GAME:
            display_text(fontSTART, fBlack, 170, 150, "1)      NEW GAME", renderer);
            break;
        
        case LOAD_GAME:
            display_text(fontSTART, fBlack, 170, 250, "2)      LOAD GAME", renderer);
            break;
        
        case QUIT:
            display_text(fontSTART, fBlack, 170, 350, "3)      QUIT", renderer);
        }
        
        

        
    }
    else if (gamestate==1) {

        SDL_Rect player_rect = {
            (int)player->x, (int)player->y,
            player->w, player->h};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_rect);

        for (size_t i = 0; i<ENEMY_NUMBER ; i++)
        {
            if(enemies[i].alive)
            {
                SDL_Rect enemy_rect = {
                (int)enemies[i].x, (int)enemies[i].y,
                enemies[i].w, enemies[i].h};

                if(enemies[i].type == 1){
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                }
                else if (enemies[i].health == 2){
                    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
                
                SDL_RenderFillRect(renderer, &enemy_rect);
            }
        }
        
        for (int i=0; i<player->health; i++)
        {
            SDL_Rect healthpool ={
                (int)SCREEN_WIDTH-10-12*(i+1), (int)10,
                16, 16};
            SDL_Surface *surface = IMG_Load("img/heart.png");
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_RenderCopy(renderer, texture, NULL, &healthpool);
        }

        if (bullet_active)
        {
            SDL_Rect bullet_rect = {
                (int)bullet->x, (int)bullet->y,
                bullet->w, bullet->h};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &bullet_rect);
        }

        if (enemy_bullet_active){
            SDL_Rect bullete_rect = {
                (int)enemy_bullet->x, (int)enemy_bullet->y,
                enemy_bullet->w, enemy_bullet->h};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &bullete_rect);
        }

        if (heart_active){
            SDL_Rect heart_rect = {
                (int)heart.x, (int)heart.y,
                heart.w, heart.h};
            SDL_Surface *surface = IMG_Load("img/heart.png");
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_RenderCopy(renderer, texture, NULL, &heart_rect);
        }
        
    }

    else if (gamestate==2)
        display_text(fontSTART, fColor, 268, 150, "YOU LOSE", renderer);

    else if (gamestate==3)
        display_text(fontSTART, fColor, 280, 150, "YOU WIN", renderer);
    
    else if (gamestate==4){
        SDL_Rect startmenu_rect = {
                        100, 50,
                        600, 500};
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderFillRect(renderer, &startmenu_rect);
        
        SDL_Rect startmenu_case = {
                        150, 150,
                        500, 50};
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &startmenu_case);
        
        display_text(fontSTART, fBlack, 170, 150, "1)      Resume", renderer);

        display_text(fontSTART, fColor, 315, 55, "PAUSE", renderer);
    }

    SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}


