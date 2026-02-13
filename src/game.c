#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>





//gestion des entrées
void handle_input(bool *running, const Uint8 *keys, Entity_player *player, Entity_bullet *bullet, bool *bullet_active, Gamestate *gamestate)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    if(*gamestate==0){
        if(keys[SDL_SCANCODE_1]){
            *gamestate=1;
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





//logique de jeu

//détermine si l'ennemi doit tirer
void enemy_tire(bool *enemy_bullet_active, Entity_bullet *enemy_bullet, int *ticks_depuis_dernier_tir, Entity_enemy *enemies)
{
    if (*ticks_depuis_dernier_tir > 500){
        *enemy_bullet_active = true;
        int i_enemies_alive[ENEMY_NUMBER];
        int nb_enemies_alive = 0;
        //on compte le nombre d'ennemis vivants et on stocke leur i dans un tableau
        for (size_t i = 0; i<ENEMY_NUMBER; i++){
            if (enemies[i].alive){
                i_enemies_alive[nb_enemies_alive] = i;
                nb_enemies_alive++;
            }
        }
        if (nb_enemies_alive==0) {
            return;
        }
        int i_tire = i_enemies_alive[rand() % nb_enemies_alive];
        enemy_bullet->x = enemies[i_tire].x + enemies[i_tire].w/2 - BULLET_WIDTH/2;
        enemy_bullet->y = enemies[i_tire].y;
        enemy_bullet->w = BULLET_WIDTH;
        enemy_bullet->h = BULLET_HEIGHT;
        enemy_bullet->vy = BULLET_SPEED/2;
        *ticks_depuis_dernier_tir = 0;
    }
}

//update toutes les positions du joueur, des ennemis et des bullet
void update(Entity_player *player, Entity_bullet *bullet, bool *bullet_active, float dt, Entity_enemy enemies[], Entity_bullet *enemy_bullet, bool *enemy_bullet_active, bool *heart_active, Entity_bullet *heart)
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

    if (*heart_active){
        heart->y += heart->vy*dt;
        if(heart->y > SCREEN_HEIGHT){
            *heart_active = false;
        }
    }

    //le mouvement des ennemis : il faut que si l'ennemi touche le bord, alors ils descendent tous + vitesse incrémentale
    bool touche_bord = false;
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        if ((enemies[i].x<0 || (enemies[i].x+ENEMY_WIDTH) > SCREEN_WIDTH) && enemies[i].alive) {
            touche_bord = true;
            break;
        }
    }
    for(size_t i=0; i<ENEMY_NUMBER; i++){
        if (touche_bord && enemies[i].alive){
            enemies[i].vx = -(enemies[i].vx*1.1);
            enemies[i].y += enemies[i].saut;
            // on remet l'ennemi dans le screen pour éviter de redétecter le même ennemi dans plusieurs frames
            if (enemies[i].x < 0)
                enemies[i].x = 0;
            if (enemies[i].x + ENEMY_WIDTH > SCREEN_WIDTH)
                enemies[i].x = SCREEN_WIDTH - ENEMY_WIDTH;
        }
        if(enemies[i].alive) {
            enemies[i].x += enemies[i].vx*dt;
        }
    }

}


//fait spawn les ennemis sur une grille
void spawn_enemies(Entity_enemy enemies[]){
    for(size_t j=0; j<ENEMY_LINES; j++)
    {
        for(size_t i=0; i<ENEMY_NUMBER_PER_LINE; i++)
        {
            size_t index = i+j*ENEMY_NUMBER_PER_LINE;
            //valeurs par défaut
            enemies[index].x = (10+ENEMY_WIDTH)*(i+1); 
            enemies[index].y = (20+ENEMY_HEIGHT)*(j+1);
            enemies[index].vx = ENEMY_SPEED;
            enemies[index].h = ENEMY_HEIGHT;
            enemies[index].w = ENEMY_WIDTH;
            enemies[index].alive = true;
            enemies[index].health = 1;
            enemies[index].saut = ENEMY_HEIGHT;

            //valeurs spécifiques aux différents types
            if (i==0 || i==ENEMY_NUMBER_PER_LINE-1){
                enemies[index].type = 1;
                enemies[index].saut = ENEMY_HEIGHT*1.9;
                enemies[index].w = ENEMY_WIDTH*0.8;
            }
            else if(j==0 || j+i%3==0){
                enemies[index].type = 2;
                enemies[index].health = 2;
            }
            else{
                enemies[index].type = 0;
            }
        }
    }
    
}

void spawn_heart(bool *heart_active, Entity_bullet *heart, Entity_enemy *enemies, size_t i){
    int spawn_or_not = rand() % 10;
    if(*heart_active==0 && spawn_or_not==0){
        *heart_active = 1;
        heart->x = enemies[i].x;
        heart->y = enemies[i].y;
        heart->vy = ENEMY_SPEED*2;
        heart->w = ENEMY_WIDTH;
        heart->h = ENEMY_HEIGHT*0.5;
    }
}

//détermine si l'ennemi est touché par le tir du joueur
void enemy_is_touched(Entity_bullet *bullet, Entity_enemy *enemies, size_t *killcount, bool *bullet_active, bool *heart_active, Entity_bullet *heart)
{

    for (size_t i=0; i<ENEMY_NUMBER; i++)
    {
        // ce long if vérifie si la balle touche bien l'ennemi et si l'ennemi est bien vivant
        if (bullet->x < (enemies[i].x + enemies[i].w) && bullet->y < (enemies[i].y + enemies[i].h) && (enemies[i].x < (bullet->x + bullet->w)) && (enemies[i].y < bullet->y + bullet->h) && (enemies[i].alive))
        {
            enemies[i].health--;
            (*bullet_active) = false; 
            if (enemies[i].health == 0)
            {
                (*killcount)++;
                enemies[i].alive = false;
                enemies[i].vx = 0;
                spawn_heart(heart_active, heart, enemies, i);
            }
        }
    }
}

//détermine si le joueur est touché par le tir de l'ennemi et enlève de la vie si c'est le cas
void player_is_touched(Entity_bullet *enemy_bullet, Entity_player *player, bool *enemy_bullet_active)
{
    if(player->x+player->w > enemy_bullet->x && player->x < enemy_bullet->x + enemy_bullet->w && player->y<enemy_bullet->y+enemy_bullet->h && player->y + player->h >enemy_bullet->y)
    {
        player->health--;
        *enemy_bullet_active = false;
    }
}

void player_is_healed(Entity_bullet *heart, Entity_player *player, bool *heart_active){
    if(player->x+player->w > heart->x && player->x < heart->x + heart->w && player->y<heart->y+heart->h && player->y + player->h >heart->y)
    {
        player->health++;
        *heart_active = false;
    }
}

//détermine si le joueur a perdu, soit parce que les ennemis ont atteint le bas, soit parce qu'il n'a plus de vie
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


