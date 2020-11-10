// state.cpp

#include "state.h"
#include "entity.h"

state::state(std::string p_name)
	: m_name(p_name)
	, m_next_state("")
{
	m_inputs = service<input_handler>::get();
	m_collisions = service<collision_handler>::get();
	m_sounds = service<sound_container>::get();
	m_music = service<music_player>::get();
	m_textures = service<texture_manager>::get();
	m_sprites = service<sprite_drawer>::get();
	m_text_maps = service<text_map_format_atlas>::get();
	m_printer = service<text_printer>::get();
}

state::~state()
{
	for(auto&& e : m_entities)
	{
		if(e != nullptr)
			delete e;
		e = nullptr;
	}
	m_entities.clear();
}

std::string state::get_next_state()
{
	return m_next_state; 
}

std::string state::get_name()
{
	 return m_name; 
}
