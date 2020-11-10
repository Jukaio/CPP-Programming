// sprite_manager.cpp

#include "sprite_drawer.h"
#include "texture.h"
#include <SDL_image.h>
#include "config.h"
#include "sdl_math.h"
#include "camera.h"

SDL_Renderer* sprite_drawer::m_renderer = nullptr;

sprite_drawer::sprite_drawer(SDL_Renderer* p_renderer)
{
	if(m_renderer == nullptr)
		m_renderer = p_renderer;
}

bool sprite_drawer::load(std::string p_id, texture* p_texture, SDL_Rect p_rect)
{
	if(p_texture == nullptr)
		return false;

	m_cache.add(p_id, new sprite(p_texture->get(), p_rect));
	return true;
}

bool sprite_drawer::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

sprite* sprite_drawer::load_and_get(std::string p_id, texture* p_texture, SDL_Rect p_rect)
{
	if(!load(p_id, p_texture, p_rect))
		return nullptr;
	return get(p_id);
}

sprite* sprite_drawer::get(std::string p_id)
{
	return m_cache[p_id];
}

void sprite_drawer::draw(sprite* p_sprite, SDL_Rect p_destination, int p_flip, double p_angle, bool to_camera)
{
	SDL_Rect camera = to_camera ? camera::instance().get_position() : SDL_Rect{ 0, 0, 0, 0 };

	SDL_Rect draw_rect = p_destination - camera;
	SDL_RenderCopyEx(m_renderer, p_sprite->get_texture(), &p_sprite->get_rect(), &draw_rect,
					 p_angle, NULL, (SDL_RendererFlip) p_flip);
}
