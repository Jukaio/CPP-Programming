// sheep.h

#ifndef INCLUDED_SHEEP_H
#define INCLUDED_SHEEP_H

#include <stdbool.h>
#include "animation.h"
#include "vector2.h"

struct SDL_Renderer;

struct node;
typedef struct node node;

struct grid;
typedef struct grid grid;

#define SHEEP_ANIMATION_COUNT 3

typedef struct sheep
{
    bool active;

    int age;
    int timer;
    int hunger;
    int health;

    int age_index;
    int anim_index;

    bool moving;
    vector2 from;
    vector2 to;
    int fraction; // * 1000; i.e. 1000 = 0.1f
    
    animation anim[SHEEP_ANIMATION_COUNT];
    SDL_Rect destination;
} sheep;

typedef struct sheep_score
{
    int max_age;
    int max_hunger;
    int max_health;
    int pool_size;
    int active_sheep;

    grid* grid_layer;
    sheep* pool;
} sheep_flock;

// TODO: Sheep eats Grass
    // if Sheep is on node with grass 
// TODO: Sheep searches Grass
    // if sheep is in radius of node with grass
// TODO: Evade Wolf 
// TODO: Sheep reproduces

void sheep_flock_init(sheep_flock* p_score,
                      grid* p_grid,
                      animation* p_anim,
                      int p_pool_size,
                      int p_max_age);
void sheep_flock_spawn_sheep(sheep_flock* p_score,
                             vector2 p_spawn_pos);
void sheep_flock_update(sheep_flock* p_score);
void sheep_flock_draw(SDL_Renderer* p_renderer,
                      sheep_flock* p_score);
void sheep_flock_clean(sheep_flock* p_score);

#endif // !INCLUDED_SHEEP_H
