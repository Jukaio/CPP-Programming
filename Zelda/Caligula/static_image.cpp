// static_image.cpp

#include "static_image.h"
#include "sprite_drawer.h"
#include "service.h"
#include <SDL_rect.h>

static_image::static_image(sprite* p_sprite, 
						   SDL_Rect p_destination, 
						   int p_scale)
{
	m_sprite = p_sprite;
	m_x = p_destination.x;
	m_y = p_destination.y;
	m_w = p_destination.w;
	m_h = p_destination.h;

	m_scale = p_scale;
}

void static_image::on_enable()
{
}

void static_image::update(int p_ms)
{
}

void static_image::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect());
}

void static_image::on_disable()
{
}

void static_image::on_collision(entity* p_other)
{
}

void static_image::reset()
{
}
