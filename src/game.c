#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>


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

void render(SDL_Renderer *renderer, Entity_player *player, Entity_bullet *bullet, bool bullet_active, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool enemy_bullet_active)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

    SDL_RenderPresent(renderer);
}



//gestion des entrées
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

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





//logique de jeu

//détermine si l'ennemi doit tirer
void enemy_tire(bool *enemy_bullet_active, Entity_bullet *enemy_bullet, int *ticks_depuis_dernier_tir, Entity_enemy *enemies)
{
    if (*ticks_depuis_dernier_tir > 500){
        *enemy_bullet_active = true;
        int i_tire = rand() % ENEMY_NUMBER;
        while (!enemies[i_tire].alive)
        {
            i_tire = rand()%ENEMY_NUMBER;
        }
        enemy_bullet->x = enemies[i_tire].x + enemies[i_tire].w/2 - BULLET_WIDTH/2;
        enemy_bullet->y = enemies[i_tire].y;
        enemy_bullet->w = BULLET_WIDTH;
        enemy_bullet->h = BULLET_HEIGHT;
        enemy_bullet->vy = BULLET_SPEED/2;
        *ticks_depuis_dernier_tir = 0;
    }
}

//update toutes les positions du joueur, des ennemis et des bullet
void update(Entity_player *player, Entity_bullet *bullet, bool *bullet_active, float dt, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool *enemy_bullet_active)
{
    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }

    if (*enemy_bullet_active){
        enemy_bullet->y += enemy_bullet->vy*dt;
        if (enemy_bullet->y > SCREEN_HEIGHT)
        {
            *enemy_bullet_active = false;
        }
    }

    bool touche_bord = false;
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        if ((enemies[i].x<0 || (enemies[i].x+ENEMY_WIDTH) > SCREEN_WIDTH) && enemies[i].alive) {
            touche_bord = true;
            break;
        }
    }
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        if (touche_bord && enemies[i].alive){
            enemies[i].vx = -(enemies[i].vx + ENEMY_SPEED*0.2);
            enemies[i].y += ENEMY_HEIGHT;
        }
        if(enemies[i].alive) {
            enemies[i].x += enemies[i].vx*dt;
        }
    }
    touche_bord = false;

}


void enemy_is_touched(Entity_bullet *bullet, Entity_enemy *enemies, size_t *killcount, bool *bullet_active)
{

    for (size_t i=0; i<ENEMY_NUMBER; i++)
    {
        // ce long if vérifie si la balle touche bien l'ennemi et si l'ennemi est bien vivant
        if (bullet->x < (enemies[i].x + enemies[i].w) && bullet->y < (enemies[i].y + enemies[i].h) && (enemies[i].x < (bullet->x + bullet->w)) && (enemies[i].y < bullet->y + bullet->h) && (enemies[i].alive))
        {
            (*killcount)++;
            enemies[i].alive = false;
            enemies[i].vx = 0;
            (*bullet_active) = false; 
        }
    }
}

void player_is_touched(Entity_bullet *enemy_bullet, Entity_player *player, bool *enemy_bullet_active)
{
    if(player->x+player->w > enemy_bullet->x && player->x < enemy_bullet->x + enemy_bullet->w && player->y<enemy_bullet->y+enemy_bullet->h && player->y + player->h >enemy_bullet->y)
    {
        player->health--;
        *enemy_bullet_active = false;
    }
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

bool has_lost(Entity_enemy *enemies, Entity_player *player){
    for (size_t i=0; i<ENEMY_NUMBER; i++){
        if (enemies[i].y>600){
            return true;
        }
    }
    if(player->health < 1){
        return true;
    }
    return false;
}
