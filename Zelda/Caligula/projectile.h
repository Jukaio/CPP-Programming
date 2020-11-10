// projectile

#ifndef INCLUDED_PROJECTILE_H
#define INCLUDED_PROJECTILE_H

#include "entity.h"

// A entity acting as a game object which gets reset on collision with certain other entities
// when "fired" it gets shot into the given direction

struct projectile : entity
{
	projectile();
	projectile(sprite* p_sprite,
			   SDL_Rect p_spawn_rect);
	~projectile();

	void fire(int start_x, int start_y, int p_dir_x, int p_dir_y, int p_speed);

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

private:
	int m_dir_x = 0; 
	int m_dir_y = 0;
	int m_speed = 0;

	// Inherited via entity
};
#endif // !INCLUDED_PROJECTILE_H



