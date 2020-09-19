// grid.

#ifndef INCLUDED_GRID_H
#define INCLUDED_GRID_H

#include <stdbool.h>
#include "vector2.h"

#define BITS 8
#define BASE 1

typedef unsigned long Uint32;
struct SDL_Renderer;

typedef enum node_origin
{
    ORIGIN_CENTER   = 0,
    ORIGIN_TOP      = 1 << 0,
    ORIGIN_RIGHT    = 1 << 1,
    ORIGIN_BOTTOM   = 1 << 2,
    ORIGIN_LEFT     = 1 << 3,
} node_origin;

enum node_data
{
    UNKNOWN         = 0,          // 0000 0000
    GRASS_LEVEL     = 7,          // 0000 0111
    HAS_GRASS       = 1 << 3,     // 0000 1000
    HAS_SHEEP       = 1 << 4,     // 0001 0000
    HAS_WOLF        = 1 << 5      // 0010 0000
};

typedef struct node
{
    unsigned char data; // 0000 0000
} node;

typedef struct grid
{
    int cell_size;
    vector2 dimensions;
    node* nodes;
    Uint32 color;
} grid;

bool grid_init(grid* out_grid, 
               vector2 p_dimension, 
               int p_cell_size, 
               Uint32 p_color);
bool grid_valid_index(vector2 p_index, 
                      vector2 p_bounds);
bool grid_set_data_at(grid* p_grid, 
                      vector2 p_index, 
                      const unsigned int bitmask_index, 
                      const unsigned int p_value);
vector2 grid_world_to_grid(grid* p_grid, 
                           vector2 p_world_pos);
vector2 grid_grid_to_world(grid* p_grid, 
                           vector2 p_grid_pos,
                           node_origin p_world_origin);
bool grid_open_neighbours_at(grid* p_grid, 
                             vector2* out_open_neighbours, 
                             int* out_open_count, 
                             vector2 p_index, 
                             enum node_data p_block_value);
int grid_check_node_data(grid* p_grid, 
                         vector2 p_index, 
                         int bitmask_index);
void grid_draw(SDL_Renderer* p_renderer, 
               grid* p_grid);
void grid_free_nodes(grid* p_grid);


#endif // !INCLUDED_GRID_H
