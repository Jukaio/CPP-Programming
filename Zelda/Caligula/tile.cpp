// tile.cpp

#include "tile.h"
#include "sprite_drawer.h"
#include "collider.h"
#include "service.h"
#include "texture_manager.h"

tile::tile()
{
	m_type = TILE_WALL;
}

tile::tile(sprite* p_sprite)
{
	m_type = TILE_WALL;
	m_sprite = p_sprite;
	m_x = 0;
	m_y = 0;
	m_w = p_sprite->get_rect().w;
	m_h = p_sprite->get_rect().h;
}

tile::~tile()
{
	if(m_collider != nullptr)
		delete m_collider;
	m_collider = nullptr;
}

void tile::set_tile_rect(const SDL_Rect& p_destination)
{
	m_x = p_destination.x;
	m_y = p_destination.y;
	m_w = p_destination.w;
	m_h = p_destination.h;
}

void tile::add_collider(SDL_Rect p_bounds)
{
	m_collider = new rectangle_collider(p_bounds.x, p_bounds.y, p_bounds.w, p_bounds.h);
}

void tile::on_enable()
{
}

void tile::update(int p_ms)
{
}

void tile::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect());
}

void tile::on_disable()
{
}

void tile::on_collision(entity* p_other)
{
}

void tile::reset()
{
}
