// title_state.cpp

#include "title_state.h"
#include "config.h"
#include "entity.h"
#include "static_image.h"

using namespace TITLE_SCREEN;

title_state::title_state(std::string p_name)
	: state(p_name)
	, m_timer(TIMER)
{
	// Initialise title background
	
	m_entities.push_back(new static_image(m_sprites->load_and_get("title_background",
																  m_textures->get("title_lore"),
																  BG_SRC_RCT), BG_DST_RCT));
}

void title_state::enter()
{
	m_music->fade_in("title", MUSIC_FADE_IN);
}

bool title_state::update(int p_ms)
{
	if(m_inputs->key_down(CONTROLS::START_BUTTON))
	{
		m_next_state = "menu";
		return false;
	}

	for(auto&& e : m_entities)
	{
		if(e->is_active())
			e->update(p_ms);
	}

	m_timer--;
	if(m_timer < 0)
	{
		m_next_state = "lore";
		return false;
	}
	return true;
}

void title_state::render()
{
	for(auto&& e : m_entities)
	{
		if(e->is_active() && e->is_visible())
			e->render();
	}
}

void title_state::exit()
{

}
