#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"
#include <time.h>

int main(void)
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Navigation navigation = {0};

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();
    srand(time(NULL));

    //initialisation des entités
    Entity_player player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .health = 3};

    Entity_bullet heart = {
        .w = ENEMY_WIDTH,
        .h = ENEMY_HEIGHT / 2
    };
    bool heart_active = false;
    
    Entity_bullet bullet = {0};
    Entity_bullet enemy_bullet = {0};
    bool bullet_active = false;
    bool enemy_bullet_active = false;
    int ticks_depuis_dernier_tir = 0;

    Niveau lvl = {
        .niv = 0,
        .nb_enemy_lines = ENEMY_LINES_0,
    };

    bool enemy_active = true;
    size_t Enemy_number_lvl = lvl.nb_enemy_lines * ENEMY_NUMBER_PER_LINE;
    // alloue pour le nb max d'ennemi (càd niveau 2)
    size_t max_enemies = (ENEMY_LINES_0 + LVL_MAX) * ENEMY_NUMBER_PER_LINE;
    Entity_enemy *enemies = malloc(max_enemies * sizeof(Entity_enemy));
    spawn_enemies(enemies, lvl);
    size_t killcount = 0;

    fontInit();

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        if (!enemy_active){
            spawn_enemies(enemies, lvl);
            enemy_active=true;
            Enemy_number_lvl = lvl.nb_enemy_lines * ENEMY_NUMBER_PER_LINE;
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active, &navigation);
        if(navigation.gamestate==1){
            update(&player, &bullet, &bullet_active, dt, enemies, &enemy_bullet, &enemy_bullet_active, &heart_active, &heart, Enemy_number_lvl);
        }

        render(renderer, &player, &bullet, bullet_active, enemies, &enemy_bullet, enemy_bullet_active, heart, heart_active, &navigation, Enemy_number_lvl);
        
        
        if (bullet_active)
        {
            enemy_is_touched(&bullet, enemies, &killcount, &bullet_active, &heart_active, &heart, Enemy_number_lvl);
        }

        if (enemy_bullet_active)
        {
            player_is_touched(&enemy_bullet, &player, &enemy_bullet_active);
        }
        else{
            ticks_depuis_dernier_tir++;
            enemy_tire(&enemy_bullet_active, &enemy_bullet, &ticks_depuis_dernier_tir, enemies, Enemy_number_lvl);
        }

        if (heart_active) //les coeurs spawnent de façon aléatoire quand le joueur tue un ennemi
        {
            player_is_healed(&heart, &player, &heart_active);
        }

//vérifie la condition de victoire
        if (killcount >= Enemy_number_lvl){
            if (lvl.niv<LVL_MAX){
                lvl.niv++;
                lvl.nb_enemy_lines = ENEMY_LINES_0 + lvl.niv;
                enemy_active = false;
                killcount = 0;
            }
            else 
                navigation.gamestate = 3;
        }
//vérifie la condition de défaite
        if (has_lost(enemies, &player, Enemy_number_lvl)){
            navigation.gamestate = 2;
        }
    }

    cleanup(window, renderer);
    free(enemies);
    return 0;
}
