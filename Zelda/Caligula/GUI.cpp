// GUI.cpp

#include "GUI.h"
#include "text_printer.h"
#include "service.h"
#include "texture_manager.h"
#include "text_map_format_atlas.h"
#include "player.h"
#include "sprite_drawer.h"

GUI::GUI(std::string p_name)
	: m_name(p_name)
{
	service<text_printer>::get()->load(m_name,
									   " ", 
									   service<texture_manager>::get()->get("font"),
									   service<text_map_format_atlas>::get()->get("white"));
}

void GUI::set_text(std::string p_text, SDL_Rect p_bounds)
{
	service<text_printer>::get()->change_string(m_name, p_text);
	m_text_x = p_bounds.x;
	m_text_y = p_bounds.y;
	m_text_w = p_bounds.w;
	m_text_h = p_bounds.h;
}

void GUI::set_hearts(sprite* p_sprite, player* p_link, int p_x, int p_y)
{
	m_heart_x = p_x;
	m_heart_y = p_y;

	m_link = p_link;

	m_heart_sprite = p_sprite;
}

void GUI::on_enable()
{
}

void GUI::update(int p_ms)
{
}

void GUI::render()
{
	service<text_printer>::get()->draw(m_name, { m_text_x, m_text_y, m_text_w, m_text_h });
	for(int i = 0; i < m_link->get_health(); i++)
	{
		SDL_Rect temp = { m_heart_x + (m_heart_sprite->get_rect().w * i),
						  m_heart_y, 
						  m_heart_sprite->get_rect().w, 
						  m_heart_sprite->get_rect().h };
		sprite_drawer::draw(m_heart_sprite, temp, 0, 0.0, false);
	}
}

void GUI::on_disable()
{
}

void GUI::on_collision(entity* p_other)
{
}

void GUI::reset()
{
}
