// health_pickup.cpp

#include "health_pickup.h"
#include "sprite_drawer.h"
#include "collider.h"
#include "sound.h"

health_pickup::health_pickup()
{
}

health_pickup::health_pickup(sprite* p_sprite, SDL_Rect p_destination, sound* p_sound)
	: m_pick_up_sound(p_sound)
{
	m_type = HEALTH_PICK_UP;
	m_sprite = p_sprite;
	m_x = p_destination.x;
	m_y = p_destination.y;
	m_w = p_destination.w;
	m_h = p_destination.h;

	m_collider = new rectangle_collider(m_x, m_y, m_w, m_h);
}

health_pickup::~health_pickup()
{
}

void health_pickup::on_enable()
{

}

void health_pickup::update(int p_ms)
{

}

void health_pickup::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect(), 0x0000, 90.0);
}

void health_pickup::on_disable()
{

}

void health_pickup::on_collision(entity* p_other)
{
	if(p_other->get_type() == LINK)
	{ 
		m_pick_up_sound->play(0);
		set_active(false);
	}
}

void health_pickup::reset()
{
	set_active(true);
}
