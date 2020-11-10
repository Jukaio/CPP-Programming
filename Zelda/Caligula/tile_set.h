// tile_set.cc

#ifndef INCLUDED_TILE_SET_H
#define INCLUDED_TILE_SET_H

// The pallet for the layer
// Holds a collection of tiles which are used as templates for the map

#include "tile.h"
#include <vector>
#include <string>

struct texture;

struct tile_set
{
	tile_set(std::string p_name, 
			 texture* p_texture, 
			 int p_set_rows, 
			 int p_set_columns, 
			 int p_tile_size);
	~tile_set();
	tile* get(int p_id);

private:
	std::vector<tile*> m_tile_set;
};

#endif // !INCLUDED_TILE_SET_H



