// sword.cpp

#include "sword.h"
#include "sprite_drawer.h"
#include "collider.h"
#include "sound.h"
#include "player.h"
#include "service.h"
#include "texture_manager.h"

sword::sword(sprite* p_north,
			 sprite* p_east,
			 sound* p_attack_sound,
			 float p_duration)
	: m_attack_sound(p_attack_sound)
	, m_north_sprite(p_north)
	, m_east_sprite(p_east)
	, m_duration((int)(p_duration * 1000))
	, m_flip(SDL_FLIP_NONE)
{
	m_sprite = m_north_sprite;
	m_collider = new rectangle_collider(0, 0, 0, 0);
	m_type = ENTITY_TYPE::SWORD;
}

sword::~sword()
{
}

void sword::set_player(player* p_link)
{
	m_link = p_link;
}

void sword::on_enable()
{
	m_timer = m_duration;
	m_attack_sound->play();
	update_position();
}

void sword::update(int p_ms)
{
	update_position();

	if(m_timer < 0)
		set_active(false);
	m_timer -= p_ms;
}

void sword::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect(), m_flip);
}

void sword::on_disable()
{
}

void sword::on_collision(entity* p_other)
{
}

void sword::update_position()
{
	auto link_rect = m_link->get_destination_rect();

	int center_x = link_rect.x + (int)(link_rect.w / 2);
	int center_y = link_rect.y - (int)(link_rect.h / 2);

	int offset_x = 0;
	int offset_y = 0;

	switch(m_link->get_direction())
	{
		case 0:
			m_sprite = m_north_sprite;
			offset_y = -8;
			m_flip = SDL_FLIP_NONE;
			break;

		case 1:
			m_sprite = m_east_sprite;
			offset_x = 8;
			m_flip = SDL_FLIP_NONE;
			break;

		case 2:
			m_sprite = m_north_sprite;
			offset_y = 8;
			m_flip = SDL_FLIP_VERTICAL;
			break;

		case 3:
			m_sprite = m_east_sprite;
			offset_x = -8;
			m_flip = SDL_FLIP_HORIZONTAL;
			break;

		default:
			break;
	}

	m_w = m_sprite->get_rect().w;
	m_h = m_sprite->get_rect().h;

	m_x = center_x - (int)(m_w / 2) + offset_x;
	m_y = center_y + (int)(m_h / 2) + offset_y;

	m_collider->set_bounds(get_destination_rect());
}

void sword::reset()
{
}
