// tile.h

#ifndef INCLUDED_TILE_H
#define INCLUDED_TILE_H

#include "entity.h"

// used entity for the tile_map_layer (layer in this case)

struct tile : entity
{
	tile();
	tile(const tile& rhs) = default;
	tile(sprite* p_sprite);
	~tile();

	void set_tile_rect(const SDL_Rect& p_destination);
	void add_collider(SDL_Rect p_bounds);

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;
};

#endif // !INCLUDED_TILE_H



