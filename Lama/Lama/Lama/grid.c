
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>
#include <stdbool.h>

#include "grid.h"
#include "input.h"

bool grid_init(grid* p_grid, vector2 p_dimension, int p_cell_size, Uint32 p_color)
{
    size_t  size        = 0;
    node*   temp_nodes  = NULL;

    if(!(((p_dimension.x * p_dimension.y) / p_cell_size) > 0) && p_grid)
        return false;

    // Create the grid outline
    p_grid->cell_size   = p_cell_size;
    p_grid->dimensions.x = (int)(p_dimension.x / p_cell_size);
    p_grid->dimensions.y = (int)(p_dimension.y / p_cell_size);
    p_grid->color       = p_color;

    size = (Uint32)(p_grid->dimensions.x * p_grid->dimensions.y);

    // Allocating the nodes for the grid
    temp_nodes = (node*)calloc(size, sizeof(node));

    // Set all node data to NULL by default
    // Assign the created nodes to the grid
    p_grid->nodes = (node*)memset(temp_nodes,
                                  NULL,
                                  sizeof(node) * size);

    return true;
}
void grid_free_nodes(grid* p_grid)
{
    free(p_grid->nodes);
    p_grid->nodes = NULL;
}
bool grid_valid_index(vector2 p_index, vector2 p_bounds)
{
    return (p_index.x * p_index.y) < (p_bounds.x * p_bounds.y);
}
bool grid_set_data_at(grid* p_grid, vector2 p_index, const unsigned int bitmask_index, const unsigned int p_value)
{
    node* temp = NULL;

    if(!grid_valid_index(p_index, p_grid->dimensions))
        return false;

    temp = &p_grid->nodes[p_index.y * p_grid->dimensions.x + p_index.x];

    // Reserve first three bits for value 1 to 7
    // Reserve other bits as bools
    temp->data = (temp->data & ~(bitmask_index));
    temp->data |= ((bitmask_index * p_value) * !(bitmask_index & p_value));
    temp->data |= (bitmask_index & p_value);
    return true;
}
vector2 grid_world_to_grid(grid* p_grid, vector2 p_world_pos)
{
    vector2 to_return;
    to_return.x = (int)(p_world_pos.x / p_grid->cell_size);
    to_return.y = (int)(p_world_pos.y / p_grid->cell_size);
    return to_return;
}
vector2 grid_grid_to_world(grid* p_grid, vector2 p_grid_pos, node_origin p_world_origin)
{
    vector2 to_return;
    vector2 offset; 
    offset.x = ((p_world_origin & ORIGIN_LEFT) * (int)(p_grid->cell_size * 0.5f)) -
               ((p_world_origin & ORIGIN_RIGHT) * (int)(p_grid->cell_size * 0.5f));
    offset.y = ((p_world_origin & ORIGIN_BOTTOM) * (int)(p_grid->cell_size * 0.5f)) -
               ((p_world_origin & ORIGIN_TOP) * (int)(p_grid->cell_size * 0.5f));

    to_return.x = (int)(p_grid_pos.x * p_grid->cell_size) + (int)(p_grid->cell_size * 0.5f);
    to_return.y = (int)(p_grid_pos.y * p_grid->cell_size) + (int)(p_grid->cell_size * 0.5f);

    to_return.x += offset.x;
    to_return.y += offset.y;

    return to_return;
}
int grid_check_node_data(grid* p_grid, vector2 p_index, int bitmask_index)
{
    return (p_grid->nodes[p_index.y * p_grid->dimensions.x + p_index.x].data & bitmask_index);
}

bool grid_open_neighbours_at(grid* p_grid, vector2* out_open_neighbours, int* out_open_count, vector2 p_index, enum node_data p_block_value)
{
    bool    is_blocked = false;

    (*out_open_count) = 0;

    // Consider edges of grid
    bool    blocked[NUM_DIRS] = { !(p_index.y > 0),
                                  !(p_index.x < (p_grid->dimensions.x - 1)),
                                  !(p_index.y < (p_grid->dimensions.y - 1)),
                                  !(p_index.x > 0) };

    // Init neighbours of center/origin
    vector2 neighbour_indeces[NUM_DIRS] = { { p_index.x + 0, p_index.y - 1 },
                                            { p_index.x + 1, p_index.y + 0 },
                                            { p_index.x - 0, p_index.y + 1 },
                                            { p_index.x - 1, p_index.y + 0 } };
    // if open_direction is index equal to center/origin, it counts as invalid
    for(int i = 0; i < NUM_DIRS; i++)
        out_open_neighbours[i] = p_index;

    // Check whether edge or neighbour blocks center/origin
    // :: Get the open neighbours of certain position
    for(int dir = 0; dir < NUM_DIRS; dir++)
    {
        is_blocked = blocked[dir] ||
            grid_check_node_data(p_grid,
                               neighbour_indeces[dir],
                               p_block_value);
        out_open_neighbours[(*out_open_count)].x = (is_blocked * p_index.x) + ((!is_blocked) * neighbour_indeces[dir].x);
        out_open_neighbours[(*out_open_count)].y = (is_blocked * p_index.y) + ((!is_blocked) * neighbour_indeces[dir].y);
        (*out_open_count) += !is_blocked;
    }
    return (*out_open_count) > NULL;
}

void grid_draw(SDL_Renderer* p_renderer, grid* p_grid)
{
    vector2     index;
    SDL_Rect    rect;
    node*       temp    = NULL;

    for(int y = 0; y < p_grid->dimensions.y; y++)
    {
        for(int x = 0; x < p_grid->dimensions.x; x++)
        {
            temp = &p_grid->nodes[y * p_grid->dimensions.x + x];
            rect.x =  x * p_grid->cell_size;
            rect.y = y * p_grid->cell_size;
            rect.w = p_grid->cell_size;
            rect.h = p_grid->cell_size;

            index.x = x;
            index.y = y;
            if(grid_check_node_data(p_grid, index, HAS_GRASS))
            {
                // To show data and sprites of objects at the same time
                SDL_Rect rect2 = rect;
                rect2.w *= 0.25f;
                rect2.h *= 0.25f;

                SDL_SetRenderDrawColor(p_renderer,
                                       0,
                                       125,
                                       0,
                                       255);
                SDL_RenderFillRect(p_renderer, &rect2);
            }
            if(grid_check_node_data(p_grid, index, HAS_SHEEP))
            {
                // To show data and sprites of objects at the same time
                SDL_Rect rect2 = rect;
                rect2.x += 32;
                rect2.w *= 0.25f;
                rect2.h *= 0.25f;

                SDL_SetRenderDrawColor(p_renderer,
                                       0,
                                       125,
                                       255,
                                       255);
                SDL_RenderFillRect(p_renderer, &rect2);
            }
            if(grid_check_node_data(p_grid, index, GRASS_LEVEL))
            {
                // To show data and sprites of objects at the same time
                SDL_Rect rect3 = rect;
                rect3.w *= 0.25f;
                rect3.x += rect3.w;

                int rest = (int) (rect3.h * (1.0f / 5.0f));

                rect3.h *= 0;
                rect3.h += rest * grid_check_node_data(p_grid, index, GRASS_LEVEL);
                SDL_SetRenderDrawColor(p_renderer,
                                       0,
                                       255,
                                       0,
                                       255);
                SDL_RenderFillRect(p_renderer, &rect3);
            }

            SDL_SetRenderDrawColor(p_renderer,
                                   (p_grid->color & 0xFF000000) >> 24,
                                   (p_grid->color & 0x00FF0000) >> 16,
                                   (p_grid->color & 0x0000FF00) >> 8,
                                   p_grid->color & 0x000000FF);

            SDL_RenderDrawRect(p_renderer, &rect);
        }
    }
}
