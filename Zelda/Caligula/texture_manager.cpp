// texture_manager.cpp

#include "texture_manager.h"

texture_manager::texture_manager(SDL_Renderer* p_renderer)
	: m_renderer(p_renderer)
{
}

texture_manager::~texture_manager()
{
	m_renderer = nullptr;
}

void texture_manager::set_alpha_channel_color(int r, int g, int b)
{
	m_color_key_set = SDL_TRUE;
	m_color_key = (Uint32)((r << 16) + (g << 8) + (b << 0));
}

bool texture_manager::load(std::string p_id, std::string p_path)
{
	SDL_Surface* surface = IMG_Load(p_path.c_str());
	if(surface == nullptr)
		return false;

	SDL_SetColorKey(surface, m_color_key_set, m_color_key);

	SDL_Texture* temp = SDL_CreateTextureFromSurface(m_renderer, surface);
	if(temp == nullptr)
		return false;

	m_cache.add(p_id, new texture(temp));
	return true;
}

bool texture_manager::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

texture* texture_manager::get(std::string p_id)
{
	return m_cache[p_id];
}
