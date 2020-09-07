// test_entity.h

#ifndef INCLUDED_TEST_ENTITY_H
#define INCLUDED_TEST_ENTITY_H

#include "entity/inheritances/updatable.h"
#include "entity/inheritances/renderable.h"
#include "entity/inheritances/collidable.h"
#include "entity/inheritances/destructable.h"

struct sprite;

struct sprite_entity : updatable, renderable, collidable, destructable
{
	sprite_entity(sprite* p_sprite,
				  vector2 p_position,
				  float p_rotation,
				  vector2 p_scale,
				  transform* p_parent = nullptr,
				  bool p_active = true,
				  bool p_visible = true);

	// Inherited via updatable
	virtual void on_enable() override;
	virtual void on_disable() override;
	virtual void update(int p_ms) override;

	// Inherited via renderable
	virtual void render(SDL_Renderer* p_renderer) override;

	// Inherited via collidable
	virtual void on_collision_enter(collidable* p_other) override;
	virtual void on_collision_stay(collidable* p_other) override;
	virtual void on_collision_exit(collidable* p_other) override;
private:
	sprite* m_sprite;

};

#endif // !INCLUDED_TEST_ENTITY_H
