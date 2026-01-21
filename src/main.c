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

    Entity_player player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0};

    Entity_bullet bullet = {0};
    bool bullet_active = false;

    //on crée les ennemis sur une seule ligne
    Entity_enemy *enemies = malloc(ENEMY_NUMBER * sizeof(Entity_enemy));
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        enemies[i].x = (10+ENEMY_WIDTH)*(i+1); 
        enemies[i].y = 10;
        enemies[i].vy = ENEMY_SPEED;
        enemies[i].h = ENEMY_HEIGHT;
        enemies[i].w = ENEMY_WIDTH;
        }

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
        update(&player, &bullet, &bullet_active, dt, enemies);
        render(renderer, &player, &bullet, bullet_active, enemies);
        if (bullet_active)
        {
            enemy_is_touched(&bullet, enemies, &killcount, &bullet_active);
        }
//vérifie la condition de victoire
        if (killcount >= ENEMY_NUMBER){
            printf("YOU WIN \n");
            running = false;
        }
//vérifie la condition de défaite
        if (has_lost(enemies)){
            printf("YOU LOSE, LOSER \n");
            running = false;
        }
    }

    cleanup(window, renderer);
    free(enemies);
    return 0;
}
