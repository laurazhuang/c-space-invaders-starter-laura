#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>

//gestion des entrées
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active, Navigation *navigation, Niveau *lvl, bool *enemy_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
        
        // Navigation du menu (pour éviter d'appuyer 20 fois sur la même touche)
        if (navigation->gamestate == 0 && event.type == SDL_KEYDOWN && !event.key.repeat)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                    if (navigation->startmenu > NEW_GAME)
                        (navigation->startmenu)--;
                    break;

                case SDL_SCANCODE_DOWN:
                    if (navigation->startmenu < QUIT)
                        (navigation->startmenu)++;
                    break;

                case SDL_SCANCODE_RETURN:
                    switch (navigation->startmenu)
                    {
                        case NEW_GAME:
                            navigation->gamestate = 1;
                            break;

                        case LOAD_GAME:
                            load_game(player, lvl, running, enemy_active);
                            navigation->gamestate = 1;
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
        else if (navigation->gamestate == 1 && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !event.key.repeat)
            navigation->gamestate=4;
        
        else if (navigation->gamestate == 4 && event.type == SDL_KEYDOWN && !event.key.repeat)
        {
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP :
                    if (navigation->pausemenu > RESUME)
                        navigation->pausemenu--;
                    break;
                case SDL_SCANCODE_DOWN :
                    if (navigation->pausemenu < QUIT_GAME)
                        navigation->pausemenu++;
                    break;
                
                case SDL_SCANCODE_RETURN :
                    switch (navigation->pausemenu)
                    {
                    case RESUME:
                        navigation->gamestate=1;
                        break;
                    
                    case SAVE_GAME:
                            save_game(player, lvl);
                            break;

                    case QUIT_GAME:
                        *running = false;
                        break;
                        
                    default:
                        break;
                    }

                default :
                    break;
            }
            
            }

    }

    if(navigation->gamestate==0){

        if(keys[SDL_SCANCODE_1]){
            navigation->gamestate=1;
        }

        if (keys[SDL_SCANCODE_2]){
            load_game(player, lvl, running, enemy_active);
            navigation->gamestate = 1;
        }

        if(keys[SDL_SCANCODE_3]){
            *running=false; 
        }
        
        
    }
    else if (navigation->gamestate==1){
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

    else if (navigation->gamestate==4){
        if (keys[SDL_SCANCODE_1])
            navigation->gamestate = 1;
        if (keys[SDL_SCANCODE_2])
            save_game(player, lvl);
        if (keys[SDL_SCANCODE_3])
            *running = false;
    }
    
}

//la sauvegarde ne sauvegarde que le nombre de points de vie du joueur ainsi que le niveau auquel il est
//(notamment, en chargeant la partie, le joueur sera re-trasnporté au début du niveau)
void save_game(Entity_player *player, Niveau *lvl) {
    FILE *file = fopen("save.txt", "w");
    fprintf(file, "%d \n", player->health);
    fprintf(file, "%ld\n", lvl->niv);
    fprintf(file, "%ld \n", lvl->nb_enemy_lines);
    fclose(file); 
}

void load_game(Entity_player *player, Niveau *lvl, bool *running, bool *enemy_active){
    FILE *file = fopen("save.txt", "r");  // "r" pour lire, pas "w" !
    if (file == NULL) {
        *running = false;
        printf("Erreur : fichier save non existant");
        return;
    }
    fscanf(file, "%d", &player->health);
    fscanf(file, "%ld", &lvl->niv);
    fscanf(file, "%ld", &lvl->nb_enemy_lines);
    fclose(file);
    *enemy_active = false;  // Force le re-spawn des ennemis
}