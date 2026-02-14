#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>

//gestion des entrées
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active, Gamestate *gamestate, Menustate *menustate)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
        
        // Navigation du menu (pour éviter d'appuyer 20 fois sur la même touche)
        if (*gamestate == 0 && event.type == SDL_KEYDOWN && !event.key.repeat)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                    if (*menustate > NEW_GAME)
                        (*menustate)--;
                    break;

                case SDL_SCANCODE_DOWN:
                    if (*menustate < QUIT)
                        (*menustate)++;
                    break;

                case SDL_SCANCODE_RETURN:
                    switch (*menustate)
                    {
                        case NEW_GAME:
                            *gamestate = 1;
                            break;
                        
                        case QUIT:
                            *running = false;
                            break;
                        
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        else if (*gamestate == 1 && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !event.key.repeat)
            *gamestate=4;
        
        else if (*gamestate == 4 && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !event.key.repeat)
            *gamestate=1;
    }

    if(*gamestate==0){

        if(keys[SDL_SCANCODE_1]){
            *gamestate=1;
        }

        if(keys[SDL_SCANCODE_3]){
            *running=false; 
        }
        
        
    }
    else if (*gamestate==1){
        player->vx = 0.0f;
        if (keys[SDL_SCANCODE_LEFT])
            player->vx = -PLAYER_SPEED;
        if (keys[SDL_SCANCODE_RIGHT])
            player->vx = PLAYER_SPEED;

        if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
        {
            *bullet_active = true;
            bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
            bullet->y = player->y;
            bullet->w = BULLET_WIDTH;
            bullet->h = BULLET_HEIGHT;
            bullet->vy = -BULLET_SPEED;
        }
    }
    
}


