// grass.c

#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "grass.h"
#include "grid.h"
#include "system_info.h"

void grass_field_init(grass_field* p_field,
                            grid* p_grid,
                            vector2 p_dimension, 
                            animation p_anim,
                            int p_grow_timer_reset_value,
                            int p_spread_timer_reset_value,
                            int p_max_life_time,
                            int p_size)
{
    grass* temp = NULL;

    p_field->dimensions                 = p_dimension;
    p_field->grow_timer_reset_value     = p_grow_timer_reset_value;
    p_field->spread_timer_reset_value   = p_spread_timer_reset_value;
    p_field->grid_layer                 = p_grid;
    p_field->max_life_time              = p_max_life_time;
    p_field->pool                       = (grass*)calloc(p_dimension.x * p_dimension.y, 
                                                         sizeof(grass));
    
    
    for(int y = 0; y < p_dimension.y; y++)
    {
        for(int x = 0; x < p_dimension.x; x++)
        {
            temp = &p_field->pool[y * p_dimension.y + x];

            temp->anim          = p_anim;
            temp->destination.x = x * p_size;
            temp->destination.y = y * p_size;
            temp->destination.w = p_size;
            temp->destination.h = p_size;
            temp->alive         = false;
            temp->timer         = 0;
            temp->grow_level    = 0;
            temp->life_time     = 0;
            temp->state = 0;
        }
    }
}

grass_input grass_sense(grass* p_grass, grid* p_grid)
{
    vector2 index = grid_world_to_grid(p_grid, *(vector2*)&p_grass->destination);
    if(grid_check_node_data(p_grid, index, HAS_WOLF) ||
       grid_check_node_data(p_grid, index, HAS_SHEEP))
        return GRASS_TRAMPLE;
    else if(p_grass->life_time <= 0)
        return GRASS_AGE_DONE;
    else if(p_grass->grow_level >= 5)
        return GRASS_FULL_GROWN;
    return GRASS_NO_INPUT;
}
                             // [input][crnt_state] 
grass_state GRASS_DECIDE_TABLE[NUM_GRASS_INPUT_CODES][NUM_GRASS_STATE_CODES] =
{   /* GROWING              SPREADING           WITHERING*/
     { GRASS_GROWING,    GRASS_SPREADING,    GRASS_WITHERING }, // NO_INPUT
     { GRASS_SPREADING,  GRASS_SPREADING,    GRASS_WITHERING }, // FULL_GROWN
     { GRASS_WITHERING,  GRASS_WITHERING,    GRASS_WITHERING }, // AGE_DONE
     { GRASS_WITHERING,  GRASS_WITHERING,    GRASS_WITHERING }, // WOLF_TRAMPLES
};

void grass_act(grass* p_grass, grass_field* p_field)
{
    if(p_grass->timer < 0)
    { 
        switch(p_grass->state)
        {
            case GRASS_GROWING:
                grass_grow(p_field->grid_layer, p_grass);
                p_grass->timer = p_field->grow_timer_reset_value;
                break;

            case GRASS_SPREADING:
                grass_spread(p_field, p_grass);
                p_grass->timer = p_field->spread_timer_reset_value;
                break;

            case GRASS_WITHERING:
                grass_wither(p_field->grid_layer, p_grass);
                p_grass->timer = p_field->grow_timer_reset_value;
                break;
        }
    }
    else if(p_grass->timer >= 0)
        p_grass->timer -= FRAME_TIME;

    p_grass->life_time -= FRAME_TIME;
}


void grass_field_update(grass_field* p_field)
{
    grass*  temp;

    for(int y = 0; y < p_field->dimensions.y; y++)
    {
        for(int x = 0; x < p_field->dimensions.x; x++)
        {
            temp = &p_field->pool[y * p_field->dimensions.x + x];
            if(!(temp->grow_level == NULL && temp->alive == false)) // If grass is entirely inactive, dont do anything 
            {
                temp->sense = grass_sense(temp, p_field->grid_layer);
                temp->state = GRASS_DECIDE_TABLE[temp->sense][temp->state];
                grass_act(temp, p_field);
            }
        }
    }
}

void grass_grow(grid* p_grid, 
                grass* p_grass)
{
    p_grass->grow_level++;
    animation_step_forward(&p_grass->anim);

    grid_set_data_at(p_grid,
                     grid_world_to_grid(p_grid, *(vector2*)&p_grass->destination), 
                     GRASS_LEVEL, 
                     p_grass->grow_level);
}

void grass_wither(grid* p_grid,
                  grass* p_grass)
{
    vector2 index = grid_world_to_grid(p_grid, *(vector2*)&p_grass->destination);
    p_grass->grow_level--;
    p_grass->alive = false;
    animation_step_backwards(&p_grass->anim);

    grid_set_data_at(p_grid,
                     index,
                     GRASS_LEVEL,
                     p_grass->grow_level);

    grid_set_data_at(p_grid,
                     index,
                     HAS_GRASS,
                     p_grass->grow_level != 0);
}

void grass_spread(grass_field* p_field,
                  grass* p_grass)
{
    vector2 adj_nodes[NUM_DIRS];
    int     open_count;

    if(grid_open_neighbours_at(p_field->grid_layer, 
                               adj_nodes, 
                               &open_count, 
                               grid_world_to_grid(p_field->grid_layer,
                                                  *(vector2*)&p_grass->destination),
                               HAS_GRASS))
    {
        vector2 random_index = adj_nodes[rand() % open_count];
        grass_field_toggle_alive_at(p_field, random_index, true);
    }
}

void grass_field_toggle_alive_at(grass_field* p_field, 
                  vector2 p_index, 
                  bool p_value)
{
    grass* temp = &p_field->pool[p_index.y * p_field->dimensions.x + p_index.x];
    if(!(temp->grow_level > 0) && p_value && !temp->alive)
    {
        temp->timer = p_field->grow_timer_reset_value;
        temp->grow_level++;
        temp->life_time = p_field->max_life_time;
        temp->alive  = true;
        animation_step_forward(&temp->anim);
        temp->state = GRASS_GROWING;
        grid_set_data_at(p_field->grid_layer, p_index, HAS_GRASS, p_value);
    }
    else if(!p_value && temp->alive)
    {
        temp->timer = p_field->grow_timer_reset_value;
        temp->alive = false;
        temp->life_time = p_field->max_life_time;
        temp->grow_level--;
        animation_step_backwards(&temp->anim);
    }
    grid_set_data_at(p_field->grid_layer, p_index, GRASS_LEVEL, temp->grow_level);
}
void grass_field_draw(SDL_Renderer* p_renderer, 
                      grass_field* p_field)
{
    grass* temp;
    for(int y = 0; y < p_field->dimensions.y; y++)
    {
        for(int x = 0; x < p_field->dimensions.x; x++)
        {
            temp = &p_field->pool[y * p_field->dimensions.x + x];
            SDL_RenderCopy(p_renderer,
                           temp->anim.texture,
                           &temp->anim.current->data.rect,
                           &temp->destination);


            switch(temp->state)
            {
                case GRASS_GROWING:
                    SDL_SetRenderDrawColor(p_renderer, 255, 255, 0, 255);
                    break;

                case GRASS_SPREADING:
                    SDL_SetRenderDrawColor(p_renderer, 255, 0, 255, 255);
                    break;

                case GRASS_WITHERING:
                    SDL_SetRenderDrawColor(p_renderer, 0, 255, 255, 255);
                    break;
            }
            SDL_Rect other_Rect = temp->destination;
            other_Rect.w /= 2;
            other_Rect.h /= 2;
            SDL_RenderFillRect(p_renderer, &other_Rect);
        }
    }
}
void grass_field_clean(grass_field* field)
{
    if(field->pool)
        free(field->pool);
    field->pool = NULL;
}