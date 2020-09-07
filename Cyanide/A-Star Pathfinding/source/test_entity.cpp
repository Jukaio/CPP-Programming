// test_entity.cpp

#include "test_entity.h"
#include "graphics/sprite.h"
#include "entity/data/transform.h"
#include <SDL_render.h>

sprite_entity::sprite_entity(sprite* p_sprite,
							 vector2 p_position,
							 float p_rotation,
							 vector2 p_scale,
							 transform* p_parent,
							 bool p_active,
							 bool p_visible)
	: updatable(p_active)
	, renderable(p_position, p_rotation, p_scale, p_parent, p_visible)
	, collidable(nullptr)
	, m_sprite(p_sprite)
{
}

void sprite_entity::update(int p_ms)
{
}

void sprite_entity::render(SDL_Renderer* p_renderer)
{
	SDL_Rect& source = *m_sprite->get_rect();

	// round to avoid float to int cast
	SDL_Rect destination =
	{
		roundf(m_transform.get_world_position().x),
		roundf(m_transform.get_world_position().y),
		source.w * m_transform.get_world_scale().x,
		source.h * m_transform.get_world_scale().y
	};

	SDL_RenderCopyEx(p_renderer,
					 m_sprite->get_texture(),
					 &source,
					 &destination,
					 m_transform.get_world_rotation(),
					 m_sprite->get_origin(),
					 (SDL_RendererFlip)m_sprite->get_flip());
}

void sprite_entity::on_collision_enter(collidable* p_other)
{
}

void sprite_entity::on_collision_stay(collidable* p_other)
{
}

void sprite_entity::on_collision_exit(collidable* p_other)
{
}

void sprite_entity::on_enable()
{
}

void sprite_entity::on_disable()
{
	
}
