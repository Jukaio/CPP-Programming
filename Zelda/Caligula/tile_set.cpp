// tile_set.cpp

#include "tile_set.h"
#include "service.h"
#include "sprite_drawer.h"

tile_set::tile_set(std::string p_name, texture* p_texture, int p_set_rows, int p_set_columns, int p_tile_size)
{
    sprite_drawer* sprites = service<sprite_drawer>::get();
    
    for(int y = 0; y < p_set_rows; y++)
    {
        for(int x = 0; x < p_set_columns; x++)
        {
            std::string name = p_name + std::to_string(x) + "_" + std::to_string(y);
            SDL_Rect rect = { x * p_tile_size, 
                              y * p_tile_size, 
                              p_tile_size, 
                              p_tile_size };

            sprite* sprite = sprites->load_and_get(name, 
                                                   p_texture, 
                                                   rect);
            tile* temp = new tile(sprite);
            m_tile_set.push_back(temp);
        }
    }
}

tile_set::~tile_set()
{
    for(auto&& tile : m_tile_set)
    {
        if(tile != nullptr)
            delete tile;
        tile = nullptr;
    }
}

tile* tile_set::get(int p_id)
{
    return m_tile_set[p_id];
}
