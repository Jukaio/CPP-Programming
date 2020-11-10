// lore_state.cpp

#include "lore_state.h"
#include "entity.h"
#include "scrolling_image.h"
#include "config.h"

#include <string>
#include <iostream>

using namespace LORE_SCREEN;

lore_state::lore_state(std::string p_name)
	: state(p_name)
{
	int index = 0;

	for(int y = 0; y < PAGES_ROWS; y++)
	{
		for(int x = 0; x < PAGES_COLUMNS; x++)
		{
			std::string sprite_name = "lore_background_" + std::to_string(index); 
			
			SDL_Rect source = BASE_SRC_RCT;
			source.x = BASE_SRC_RCT.x + ((BASE_SRC_RCT.w + SRC_OFFSET) * x);
			source.y = BASE_SRC_RCT.y + ((BASE_SRC_RCT.h + SRC_OFFSET) * y);

			m_sprites->load(sprite_name,
							m_textures->get("title_lore"),
							source);

			SDL_Rect destination = BASE_DST_RECT;
			destination.y = CONFIG::UNSCALED_SCREEN_HEIGHT + (CONFIG::UNSCALED_SCREEN_HEIGHT * index);
			scrolling_image* img = new scrolling_image(m_sprites->get(sprite_name),
													   destination,
													   SRCOLL_SPEED_X, 
													   SCROLL_SPEED_Y);
			m_entities.push_back(img);
			index++;
		}
	}
	m_last_page = m_entities[m_entities.size() - 1];
}

void lore_state::enter()
{
}

bool lore_state::update(int p_ms)
{
	if(m_inputs->key_down(CONTROLS::START_BUTTON))
	{
		m_next_state = "menu";
		return false;
	}

	if(m_last_page->get_position_y() >= SCROLL_STOP)
	{ 
		for(auto&& e : m_entities)
		{
			if(e->is_active())
				e->update(p_ms);
		}
	}
	return true;
}

void lore_state::render()
{
	for(auto&& e : m_entities)
	{
		if(e->is_active() && e->is_visible())
			e->render();
	}
}

void lore_state::exit()
{
	
}
