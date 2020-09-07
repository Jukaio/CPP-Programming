// sprite_handler.h

#ifndef INCLUDED_SPRITE_HANDLER_H
#define INCLUDED_SPRITE_HANDLER_H

#include "utilities/cyanide_math.h"
#include "utilities/resource_cache.h"
#include "sprite.h"

 struct transform;
 struct SDL_Renderer;

struct sprite_handler
{
	struct sprite_info
	{
		int m_id;
		sprite* m_sprite;
	};

	sprite_handler();
	~sprite_handler();

	sprite_info create_sprite(texture* p_texture,
							  const rectangle& p_rect,
							  const vector2& p_relative_origin = { 0.5f, 0.5f },
							  sprite_flip p_flip = sprite_flip::NONE);
	static void draw(SDL_Renderer* p_renderer, sprite* p_sprite, transform* p_transform);

private:
	resource_cache<int, sprite> m_cache;
	static int m_size;
	
};

#endif // !INCLUDED_SPRITE_HANDLER_H



