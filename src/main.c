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

    Entity bullet = {0};
    bool bullet_active = false;

    //on crée les ennemis sur une seule ligne
    Entity enemies[ENEMY_NUMBER];
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        enemies[i].x = (10+ENEMY_WIDTH)*(i+1); 
        enemies[i].y = 10;
        enemies[i].vy = ENEMY_SPEED;
        enemies[i].vx = 0;
        enemies[i].h = ENEMY_HEIGHT;
        enemies[i].w = ENEMY_WIDTH;
        }


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
    }

    cleanup(window, renderer);
    return 0;
}
