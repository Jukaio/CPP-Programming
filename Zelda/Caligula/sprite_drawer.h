// sprite_manager.h

#ifndef INCLUDED_SPRITE_MANAGER_H
#define INCLUDED_SPRITE_MANAGER_H

#include "resource_cache.h"
#include "sprite.h"

// stores sprites in a cache and owns a static "draw function" which requires a static pointer to the renderer
// this class/struct stores sprites and draws/renders themn

// The sprite_drawer::draw function is static, since it will never use memeber data, but it is part of the 
// sprite_drawer to ensure global calls - This functionality could have gotten added in the sprite class

struct texture;
struct SDL_Rect;
struct SDL_Renderer;


struct sprite_drawer
{
	sprite_drawer(SDL_Renderer* p_renderer);
	bool load(std::string p_id, texture* p_texture, SDL_Rect p_rect);
	bool unload(std::string p_id);

	sprite* load_and_get(std::string p_id, texture* p_texture, SDL_Rect p_rect);
	sprite* get(std::string p_id);
	
	static void draw(sprite* p_sprite, SDL_Rect p_destination, int p_flip = 0, double p_angle = NULL, bool to_camera = true);

private:
	resource_cache<sprite> m_cache;
	static SDL_Renderer* m_renderer;
};

#endif // !INCLUDED_SPRITE_MANAGER_H



