#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
} Entity;

typedef struct
{
    float x, y;
    float vx;
    int w, h;
    int health;
} Entity_player;


typedef struct
{
    float x, y;
    float vy;
    int w, h;
} Entity_bullet;

typedef enum
{
    NORMAL,
    RAPIDE,
    RESISTANT,
    TIREUR,
} Cat_enemy;


typedef struct
{
    float x, y;
    float vx;
    int w, h;
    int health, saut;
//on va dire que type = 0 c'est un ennemi normal, =1 rapide, =2 résistant, =3 tire plus fréquemment
    Cat_enemy type;
    bool alive;
} Entity_enemy;

typedef enum {
    START_MENU,
    GAME,
    LOST,
    WIN,
} Gamestate;

#endif

