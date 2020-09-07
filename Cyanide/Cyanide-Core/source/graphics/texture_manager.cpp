// texture_manager.cpp

#include "graphics/texture_manager.h"
#include <iostream>
#include <assert.h>
#include <SDL_image.h>

texture_manager::texture_manager()
{
}

texture_manager::~texture_manager()
{

}

void texture_manager::set_alpha_channel_color(int r, int g, int b)
{
	m_color_key_set = true;
	m_color_key = (uint32_t)((r << 16) + (g << 8) + (b << 0));
}

texture* texture_manager::load(SDL_Renderer* p_renderer, const std::string& p_path)
{
	if(m_cache.exists(p_path))
	{
		return m_cache.get(p_path);
	}

	SDL_Surface* surface = IMG_Load(p_path.c_str());
	if(surface == nullptr)
	{ 
		assert(!"Error: Texture in path not found\n");
		std::cout << "File in path " << p_path << " not found!\n";
		return nullptr;
	}
	SDL_SetColorKey(surface, m_color_key_set, m_color_key);

	SDL_Texture* temp = SDL_CreateTextureFromSurface(p_renderer, surface);
	if(temp == nullptr)
	{
		assert(!"Error: Texture corrupted\n");
		std::cout << "Error in path " << p_path << "!\n";
		return nullptr;
	}
	texture* to_return = new texture(temp);
	m_cache.add(p_path, to_return);
	return to_return;
}

texture* texture_manager::get(const std::string& p_path)
{
	if(m_cache.exists(p_path))
	{
		return m_cache.get(p_path);
	}
	return nullptr;
}

bool texture_manager::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

