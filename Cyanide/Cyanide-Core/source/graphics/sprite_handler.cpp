// sprite_handler.cpp

#include "graphics/sprite_handler.h"
#include "entity/data/transform.h"
#include <SDL_render.h>
#include "graphics/texture.h"

int sprite_handler::m_size = 0;

sprite_handler::sprite_handler()
{
}

sprite_handler::~sprite_handler()
{
}

sprite_handler::sprite_info sprite_handler::create_sprite(texture* p_texture, const rectangle& p_rect, const vector2& p_relative_origin, sprite_flip p_flip)
{
	sprite* spr = new sprite(p_texture,
							 p_rect,
							 p_relative_origin,
							 p_flip);
	m_cache.add(m_size, spr);
	m_size++;
	return { m_size - 1, spr };
}

void sprite_handler::draw(SDL_Renderer* p_renderer, sprite* p_sprite, transform* p_transform)
{
	SDL_Rect source = { p_sprite->get_rect()->x,
						p_sprite->get_rect()->y,
						p_sprite->get_rect()->w,
						p_sprite->get_rect()->h };

		// round to avoid float to int cast
	SDL_Rect destination =
	{
		roundf(p_transform->get_world_position().x),
		roundf(p_transform->get_world_position().y),
		source.w * p_transform->get_world_scale().x,
		source.h * p_transform->get_world_scale().y
	};

	SDL_RenderCopyEx(p_renderer,
					 p_sprite->get_texture()->get(),
					 &source,
					 &destination,
					 p_transform->get_world_rotation(),
					 (SDL_Point*) p_sprite->get_origin(),
					 (SDL_RendererFlip)p_sprite->get_flip());
}
