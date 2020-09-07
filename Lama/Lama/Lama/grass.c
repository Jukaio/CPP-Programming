// grass.c

#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "grass.h"
#include "grid.h"

void grass_field_init(grass_field* p_field,
                            grid* p_grid,
                            vector2 p_dimension, 
                            SDL_Rect p_source_rect,
                            int p_grow_timer_reset_value,
                            int p_spread_timer_reset_value,
                            int p_animation_step, 
                            int p_size)
{
    grass* temp = NULL;

    p_field->dimensions                 = p_dimension;
    p_field->grow_timer_reset_value     = p_grow_timer_reset_value;
    p_field->source_index_step          = p_animation_step;
    p_field->spread_timer_reset_value   = p_spread_timer_reset_value;
    p_field->grid_layer                 = p_grid;
    p_field->pool                       = (grass*)calloc(p_dimension.x * p_dimension.y, 
                                                         sizeof(grass));

    for(int y = 0; y < p_dimension.y; y++)
    {
        for(int x = 0; x < p_dimension.x; x++)
        {
            temp = &p_field->pool[y * p_dimension.y + x];

            temp->source = p_source_rect;

            temp->destination.x = x * p_size;
            temp->destination.y = y * p_size;
            temp->destination.w = p_size;
            temp->destination.h = p_size;

            temp->timer = 0;
            temp->grow_level = 0;
        }
    }
}
void grass_field_update(grass_field* p_field)
{
    grass*  temp;
    vector2 index;

    for(int y = 0; y < p_field->dimensions.y; y++)
    {
        for(int x = 0; x < p_field->dimensions.x; x++)
        {
            temp = &p_field->pool[y * p_field->dimensions.x + x];

            if(temp->grow_level > 0 && 
               temp->grow_level < 5 &&
               temp->timer < 0)
            {
                index.x = x;
                index.y = y;
                grass_grow(p_field, temp, index);
                temp->timer = p_field->grow_timer_reset_value + 
                              ((temp->grow_level >= 5) * 
                              p_field->spread_timer_reset_value);
            }
            else if(temp->grow_level >= 5 &&
                    temp->timer < 0)
            {
                index.x = x;
                index.y = y;
                grass_spread(p_field, index);
                temp->timer = p_field->spread_timer_reset_value;
            }
            else if(temp->timer >= 0)
                temp->timer--;
        }
    }
}
void grass_grow(grass_field* p_field, 
                grass* p_grass, 
                vector2 p_index)
{
    p_grass->grow_level++;
    p_grass->source.x += p_field->source_index_step;;

    grid_set_data_at(p_field->grid_layer, 
                     p_index, GRASS_LEVEL, 
                     p_grass->grow_level);
}

void grass_spread(grass_field* p_field,
                 vector2 p_index)
{
    vector2 adj_nodes[NUM_DIRS];
    int     open_count;

    if(grid_open_neighbours_at(p_field->grid_layer, adj_nodes, &open_count, p_index, HAS_GRASS))
    {
        vector2 random_index = adj_nodes[rand() % open_count];
        grass_field_toggle_active_at(p_field, random_index, true);
    }
}

void grass_field_toggle_active_at(grass_field* p_field, 
                  vector2 p_index, 
                  bool p_value)
{
    grass* temp = &p_field->pool[p_index.y * p_field->dimensions.x + p_index.x];
    if(!(temp->grow_level > 0) && p_value)
    {
        temp->timer         = p_field->grow_timer_reset_value;
        temp->source.x      += p_field->source_index_step;
        temp->source.x      *= p_value;
        temp->grow_level++;
    }
    else if(!p_value)
    {
        temp->timer      = NULL;
        temp->source.x   = NULL;
        temp->grow_level = NULL;
    }
    grid_set_data_at(p_field->grid_layer, p_index, HAS_GRASS, p_value);
    grid_set_data_at(p_field->grid_layer, p_index, GRASS_LEVEL, temp->grow_level);
}
void grass_field_draw(SDL_Renderer* p_renderer, 
                      SDL_Texture* p_texture, 
                      grass_field* p_field)
{
    grass* temp;
    for(int y = 0; y < p_field->dimensions.y; y++)
    {
        for(int x = 0; x < p_field->dimensions.x; x++)
        {
            temp = &p_field->pool[y * p_field->dimensions.x + x];
            SDL_RenderCopy(p_renderer,
                           p_texture,
                           &temp->source,
                           &temp->destination);
        }
    }
}
void grass_field_clean(grass_field* field)
{
    if(field->pool)
        free(field->pool);
    field->pool = NULL;
}