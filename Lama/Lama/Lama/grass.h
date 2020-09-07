// grass.h

#ifndef INCLUDED_GRASS_H
#define INCLUDED_GRASS_H

#include <SDL_rect.h>
#include <stdbool.h>
#include "vector2.h"

struct SDL_Renderer;
struct SDL_Texture;

typedef struct grid grid;

typedef struct grass
{
    int grow_level;
    int timer;

    SDL_Rect source;
    SDL_Rect destination;
} grass;

typedef struct grass_field
{
    // AI stuff?
    int grow_timer_reset_value;
    int spread_timer_reset_value;

    int source_index_step; // grass' index step (animation)
    grid* grid_layer;
    grass* pool;
    vector2 dimensions;

} grass_field;

void grass_field_init(grass_field* p_field,
                            grid* p_grid,
                            vector2 p_dimension, 
                            SDL_Rect p_source_rect, 
                            int p_grow_timer_reset_value,
                            int p_spread_timer_reset_value,
                            int p_animation_step, 
                            int p_size);
void grass_field_update(grass_field* p_field);
void grass_grow(grass_field* p_field,  
                grass* p_grass, 
                vector2 p_index);
void grass_spread(grass_field* p_field, 
                 vector2 p_index);
void grass_field_toggle_active_at(grass_field* p_field,
                                  vector2 p_index, 
                                  bool p_value);
void grass_field_draw(SDL_Renderer* p_renderer, 
                      SDL_Texture* p_texture, 
                      grass_field* p_field);
void grass_field_clean(grass_field* p_field);



#endif // !INCLUDED_GRASS_H
