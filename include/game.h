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
#define ENEMY_LINES_0 5
#define LVL_MAX 2
#define HEALTH_MAX 5


void spawn_enemies(Entity_enemy enemies[], Niveau lvl);
bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active, Navigation *navigation, Niveau *lvl, bool *enemy_active);
void update(Entity_player *player, Entity_bullet *bullet, bool *bullet_active, float dt, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool *enemy_bullet_active, bool *heart_active, Entity_bullet *heart, size_t enemy_number_lvl);
void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool enemy_bullet_active, Entity_bullet heart, bool heart_active, Navigation *navigation, size_t enemy_number_lvl, Niveau lvl);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void enemy_is_touched(Entity_bullet *bullet, Entity_enemy *enemies, size_t *killcount, bool* bullet_active, bool *heart_active, Entity_bullet *heart, size_t enemy_number_lvl);
bool has_lost(Entity_enemy *enemies, Entity_player *player, size_t enemy_number_lvl);
void enemy_tire(bool *enemy_bullet_active, Entity_bullet *enemy_bullet, int *ticks_depuis_dernier_tir, Entity_enemy *enemies, size_t enemy_number_lvl);
void player_is_touched(Entity_bullet *enemy_bullet, Entity_player *player, bool *enemy_bullet_active);
void player_is_healed(Entity_bullet *heart, Entity_player *player, bool *heart_active);
void spawn_heart(bool *heart_active, Entity_bullet *heart, Entity_enemy *enemies, size_t i);
void fontInit();
void save_game(Entity_player *player, Niveau *lvl);
void load_game(Entity_player *player, Niveau *lvl, bool *running, bool *enemy_active);

#endif
