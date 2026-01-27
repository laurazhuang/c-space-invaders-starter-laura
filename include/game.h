#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include <time.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 20
#define ENEMY_SPEED 50.0f
#define ENEMY_NUMBER_PER_LINE 10
#define ENEMY_LINES 4
#define ENEMY_NUMBER ENEMY_NUMBER_PER_LINE*ENEMY_LINES


void spawn_enemies(Entity_enemy *enemies);
bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active);
void update(Entity_player *player, Entity_bullet *bullet, bool *bullet_active, float dt, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool *enemy_bullet_active);
void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool enemy_bullet_active);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void enemy_is_touched(Entity_bullet *bullet, Entity_enemy *enemies, size_t *killcount, bool* bullet_active);
bool has_lost(Entity_enemy *enemies, Entity_player *player);
void enemy_tire(bool *enemy_bullet_active, Entity_bullet *enemy_bullet, int *ticks_depuis_dernier_tir, Entity_enemy *enemies);
void player_is_touched(Entity_bullet *enemy_bullet, Entity_player *player, bool *enemy_bullet_active);

#endif
