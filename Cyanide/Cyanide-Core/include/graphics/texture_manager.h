// texture_manager.h

#ifndef INCLUDED_TEXTURE_MANAGER_H
#define INCLUDED_TEXTURE_MANAGER_H

#include "utilities/resource_cache.h"
#include "texture.h"

struct SDL_Renderer;

// Texture and collection functionality
// Saves all textures which are getting loaded
// Also allows setting a alpha_channel_colour for .jpgs, .bmps, etc.

struct texture_manager
{
	texture_manager();
	~texture_manager();

	void set_alpha_channel_color(int r, int g, int b);

	texture* load(SDL_Renderer* p_renderer, const std::string& p_path);
	texture* get(const std::string& p_path);

	bool unload(std::string p_id);

private:
	resource_cache<std::string, texture> m_cache;

	uint32_t m_color_key;
	bool m_color_key_set = false;
};

#endif // !INCLUDED_TEXTURE_MANAGER_H
