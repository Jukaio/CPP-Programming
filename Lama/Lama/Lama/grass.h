// grass.h

#ifndef INCLUDED_GRASS_H
#define INCLUDED_GRASS_H

#include <stdbool.h>

#include "vector2.h"
#include "animation.h"

struct SDL_Renderer;
struct SDL_Texture;
struct grass;

typedef struct grid grid;


typedef enum grass_input
{
    GRASS_NO_INPUT,
    GRASS_FULL_GROWN,
    GRASS_AGE_DONE,
    GRASS_TRAMPLE,
    //
    NUM_GRASS_INPUT_CODES
} grass_input;

typedef enum grass_state
{
    ERROR = -1,
    GRASS_GROWING,
    GRASS_SPREADING,
    GRASS_WITHERING,
    //
    NUM_GRASS_STATE_CODES
} grass_state;




typedef struct grass
{
    bool alive;
    int grow_level;
    int timer;
    int life_time;

    animation anim;
    SDL_Rect destination;

    int sense;
    grass_state state;
} grass;


typedef struct grass_field
{
    // AI stuff?
    int grow_timer_reset_value;
    int spread_timer_reset_value;
    int max_life_time;

    grid* grid_layer;
    grass* pool;
    vector2 dimensions;

} grass_field;

void grass_field_init(grass_field* p_field,
                            grid* p_grid,
                            vector2 p_dimension, 
                            animation p_anim, 
                            int p_grow_timer_reset_value,
                            int p_spread_timer_reset_value,
                            int p_max_life_time,
                            int p_size);

grass_input grass_sense(grass* p_grass, grid* p_grid);
void grass_act(grass* p_grass, grass_field* p_field);

void grass_field_update(grass_field* p_field);
void grass_grow(grid* p_grid,
                grass* p_grass);
void grass_wither(grid* p_grid,
                  grass* p_grass);
void grass_spread(grass_field* p_field,
                  grass* p_grass);
void grass_field_toggle_alive_at(grass_field* p_field,
                                  vector2 p_index, 
                                  bool p_value);
void grass_field_draw(SDL_Renderer* p_renderer, 
                      grass_field* p_field);
void grass_field_clean(grass_field* p_field);



#endif // !INCLUDED_GRASS_H
