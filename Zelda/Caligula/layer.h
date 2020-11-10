// layer.h

#ifndef INCLUDED_LAYER_H
#define INCLUDED_LAYER_H

#include "entity.h"
#include "tile_set.h"

// layer of a tile_map - Can get reused for multiple tile-based layers,
// ground, walls, yadda yadda
// labda or a supporting class could get used to implement a factory for a layer
// Would enable an easier creation based on input files 

struct layer : entity
{
	layer(tile_set* p_tile_set, 
		  int p_width, 
		  int p_height, 
		  int tile_width, 
		  int tile_height,
		  std::vector<int>& tile_data,
		  int p_id_offset, 
		  int p_offset_x, 
		  int p_offset_y);
	~layer();

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

	tile* get_tile(int p_id);
	int size();

private:
	std::vector<tile*> m_map;


	// Inherited via entity

};

#endif // !INCLUDED_LAYER_H