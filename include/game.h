#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"



#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMY_WIDTH 20
#define ENEMY_HEIGHT 20
#define ENEMY_SPEED 10.0f
#define ENEMY_NUMBER 10

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active);
void update(Entity_player *player, Entity_bullet *bullet, bool *bullet_active, float dt, Entity_enemy enemies[]);
void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[]);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void enemy_is_touched(Entity_bullet *bullet, Entity_enemy *enemies, size_t killcount);

#endif
