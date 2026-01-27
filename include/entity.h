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



typedef struct
{
    float x, y;
    float vx;
    int w, h;
    bool alive;
} Entity_enemy;

#endif

