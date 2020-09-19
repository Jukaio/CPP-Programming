// sheep.c

#include <stdlib.h>
#include <string.h>
#include <SDL_render.h>

#include "sheep.h"
#include "grid.h"

#include "system_info.h"

void sheep_flock_init(sheep_flock* p_flock,
                      grid* p_grid,
                      animation* p_anim,
                      int p_pool_size,
                      int p_max_age)
{
    int index;
    int anim_index;
    SDL_Rect default_dest = { 0, 0, 64, 64};
    p_flock->pool = (sheep*) calloc(sizeof(sheep), p_pool_size);
    p_flock->pool_size = p_pool_size;
    p_flock->grid_layer = p_grid;
    p_flock->max_age = p_max_age;
    memset(p_flock->pool, NULL, sizeof(sheep) * p_pool_size);
    

    for(index = 0; index < p_pool_size; index++)
        for(anim_index = 0; anim_index < SHEEP_ANIMATION_COUNT; anim_index++)
        {
            p_flock->pool[index].anim[anim_index] = p_anim[anim_index];
            p_flock->pool[index].destination = default_dest;
        }
}
void sheep_flock_spawn_sheep(sheep_flock* p_flock, vector2 p_spawn_pos)
{
    sheep* temp;

    if(p_flock->pool_size <= p_flock->active_sheep)
        return;
    for(int i = 0; i < p_flock->pool_size; i++)
    {
        temp = &p_flock->pool[i];
        if(!p_flock->pool[i].active)
        {
            temp->destination.x = p_spawn_pos.x;
            temp->destination.y = p_spawn_pos.y;
            temp->age = 0;
            temp->health = p_flock->max_health;
            temp->hunger = p_flock->max_hunger;
            temp->active = true;
            p_flock->active_sheep++;

            grid_set_data_at(p_flock->grid_layer,
                             grid_world_to_grid(p_flock->grid_layer,
                                                *((vector2*)&temp->destination)),
                             HAS_SHEEP, true);
            return;
        }
    }
}

bool sheep_move(sheep* p_sheep)
{

    vector2 next = lerp(p_sheep->from,
                        p_sheep->to,
                        p_sheep->fraction);

    p_sheep->destination.x = next.x;
    p_sheep->destination.y = next.y;
    p_sheep->fraction += FRAME_TIME;

    return interpolate(0, 1000, &p_sheep->fraction);
}


void sheep_flock_update(sheep_flock* p_flock)
{
    int index;
    sheep* temp;
    vector2 position;
    vector2 grid_index;
    vector2 open_indeces[NUM_DIRS];
    vector2 random_index;
    int open_count;

    for(index = 0; index < p_flock->pool_size; index++)
    {
        temp = &p_flock->pool[index];
        if(temp->active)
        {
            position = *((vector2*) &temp->destination);
            grid_index = grid_world_to_grid(p_flock->grid_layer,
                                            position);

            if(!temp->moving)
            {
                if(grid_open_neighbours_at(p_flock->grid_layer,
                                           open_indeces,
                                           &open_count,
                                           grid_index,
                                           HAS_SHEEP))
                {
                    temp->from = position;

                    grid_set_data_at(p_flock->grid_layer,
                                     grid_world_to_grid(p_flock->grid_layer,
                                     temp->from),
                                     HAS_SHEEP, false);

                    random_index = open_indeces[rand() % open_count];
                    position.x = random_index.x;
                    position.y = random_index.y;

                    position = grid_grid_to_world(p_flock->grid_layer,
                                                  position,
                                                  ORIGIN_CENTER);
                    temp->to = position;
                    grid_set_data_at(p_flock->grid_layer,
                                     grid_world_to_grid(p_flock->grid_layer,
                                                        temp->to),
                                     HAS_SHEEP, true);
                    temp->fraction = 0;
                    temp->moving = true;
                }
            }
            else if(temp->moving)
            {
                if(sheep_move(temp))
                {
                    temp->moving = false;
                }

            }

            temp->age += FRAME_TIME;
            temp->age_index = temp->age / (p_flock->max_age / SHEEP_ANIMATION_COUNT);
            if(temp->age > p_flock->max_age)
            {
                // Let sheep die
            }
        }
    }
}

void sheep_flock_draw(SDL_Renderer* p_renderer,
                      sheep_flock* p_flock)
{
    int index;
    sheep* temp;
    SDL_Rect offset_destination;

    for(index = 0; index < p_flock->pool_size; index++)
    {
        temp = &p_flock->pool[index];
        if(temp->active)
        {
            offset_destination = temp->destination;
            offset_destination.x -= (int)(offset_destination.w * 0.5f);
            offset_destination.y -= (int)(offset_destination.h * 0.5f);
            SDL_RenderCopy(p_renderer,
                           temp->anim[temp->anim_index].texture,
                           &temp->anim[temp->anim_index].frames[temp->age_index].data.rect,
                           &offset_destination);
        }
    }
}
void sheep_flock_clean(sheep_flock* p_flock)
{
    if(p_flock->pool)
        free(p_flock->pool);
    p_flock->pool = NULL;
}
