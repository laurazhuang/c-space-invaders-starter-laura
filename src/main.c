#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();
    srand(time(NULL));

    Entity_player player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .health = 3};

    Entity_bullet bullet = {0};
    Entity_bullet enemy_bullet = {0};
    bool bullet_active = false;
    bool enemy_bullet_active = false;
    int ticks_depuis_dernier_tir = 0;

    //on crée les ennemis sur une seule ligne
    Entity_enemy *enemies = malloc(ENEMY_NUMBER * sizeof(Entity_enemy));
    spawn_enemies(enemies);

    size_t killcount = 0;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        update(&player, &bullet, &bullet_active, dt, enemies, &enemy_bullet, &enemy_bullet_active);
        render(renderer, &player, &bullet, bullet_active, enemies, &enemy_bullet, enemy_bullet_active);
        if (bullet_active)
        {
            enemy_is_touched(&bullet, enemies, &killcount, &bullet_active);
        }

        if (enemy_bullet_active)
        {
            player_is_touched(&enemy_bullet, &player, &enemy_bullet_active);
        }
        else{
            ticks_depuis_dernier_tir++;
            enemy_tire(&enemy_bullet_active, &enemy_bullet, &ticks_depuis_dernier_tir, enemies);
        }

//vérifie la condition de victoire
        if (killcount >= ENEMY_NUMBER){
            printf("YOU WIN \n");
            running = false;
        }
//vérifie la condition de défaite
        if (has_lost(enemies, &player)){
            printf("YOU LOSE, LOSER \n");
            running = false;
        }
    }

    cleanup(window, renderer);
    free(enemies);
    return 0;
}
