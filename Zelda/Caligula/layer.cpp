//layer.cpp

#include "layer.h"

layer::layer(tile_set* p_tile_set,
			 int p_width,
			 int p_height,
			 int p_tile_width,
			 int p_tile_height,
             std::vector<int>& p_map_ids,
             int p_id_offset, 
             int p_offset_x, 
             int p_offset_y)
{
    m_x = p_offset_x;
    m_y = p_offset_y;

    int index = 0;
    for(int y = 0; y < p_height; y++)
    {
        for(int x = 0; x < p_width; x++)
        {
            int id = p_map_ids[index] - p_id_offset;
            tile* temp = new tile(*p_tile_set->get(id));

            SDL_Rect rect = { x * p_tile_width + p_offset_x, 
                              y * p_tile_height + p_offset_y, 
                              p_tile_width, 
                              p_tile_height };
            temp->set_tile_rect(rect);
            
            if(id == 20) // TODO: List of flags(?)
                temp->add_collider(rect);

            m_map.push_back(temp);
            index++;
        }
    }
}

layer::~layer()
{
    for(auto&& tile : m_map)
    {
        if(tile != nullptr)
            delete tile;
        tile = nullptr;
    }
}

void layer::on_enable()
{
}

void layer::update(int p_ms)
{
}

void layer::render()
{
    for(auto&& tile : m_map)
    {
        tile->render();
    }
}

void layer::on_disable()
{
}

void layer::on_collision(entity* p_other)
{
}

tile* layer::get_tile(int p_id)
{
    return m_map[p_id];
}

int layer::size()
{
    return m_map.size();
}

void layer::reset()
{
}
