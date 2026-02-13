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

void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool enemy_bullet_active, Gamestate gamestate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gamestate==0){
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
        
        SDL_Surface* fontSurfaceStart = TTF_RenderText_Solid(fontSTART, "1)  START", fBlack);
                if (fontSurfaceStart) {
                    SDL_Texture* fontTextureStart = SDL_CreateTextureFromSurface(renderer, fontSurfaceStart);
                    SDL_Rect fontRectStart = {150, 150, fontSurfaceStart->w, fontSurfaceStart->h};
                    SDL_RenderCopy(renderer, fontTextureStart, NULL, &fontRectStart);
                    SDL_DestroyTexture(fontTextureStart);
                    SDL_FreeSurface(fontSurfaceStart);
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
                10, 20};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &healthpool);
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
    }
    else if (gamestate==2){
        SDL_Rect startmenu_rect = {
                        100, 50,
                        600, 500};
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &startmenu_rect);
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
