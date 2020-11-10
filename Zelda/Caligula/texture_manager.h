// texture_manager.h

#ifndef INCLUDED_TEXTURE_MANAGER_H
#define INCLUDED_TEXTURE_MANAGER_H

#include "resource_cache.h"
#include "texture.h"
#include <SDL_image.h>

// Texture and collection functionality
// Saves all textures which are getting loaded
// Also allows setting a alpha_channel_colour for .jpgs, .bmps, etc.
// it is a simple implementation which requires the user not to load the same texture twice,
// TODO: Enxure that a path only gets loaded once

struct texture_manager
{
	texture_manager(SDL_Renderer* p_renderer);
	~texture_manager();

	void set_alpha_channel_color(int r, int g, int b);

	bool load(std::string p_id, std::string p_path);
	bool unload(std::string p_id);
	texture* get(std::string p_id);

	SDL_Renderer* m_renderer;
private:
	resource_cache<texture> m_cache;
	Uint32 m_color_key;
	SDL_bool m_color_key_set = SDL_FALSE;
};

#endif // !INCLUDED_TEXTURE_MANAGER_H



