// game.cpp

#include "game.h"

game::game(SDL_Renderer* p_renderer)
	: m_renderer(p_renderer)
	, m_sprite_container(p_renderer)
	, m_textures_manager(texture_manager(p_renderer))
	, m_text_printer(text_printer(p_renderer))
	, m_running(false)
{
	// Sets all services
	service<input_handler>::set(&m_input_handler);
	service<collision_handler>::set(&m_collisions_handler);
	service<sound_container>::set(&m_sound_container);
	service<music_player>::set(&m_music_player);
	service<texture_manager>::set(&m_textures_manager);
	service<sprite_drawer>::set(&m_sprite_container);
	service<text_map_format_atlas>::set(&m_text_map_format_atlas);
	service<text_printer>::set(&m_text_printer);
}
