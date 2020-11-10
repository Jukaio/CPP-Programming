// projectile.cpp

#include "projectile.h"
#include "collider.h"
#include "sprite_drawer.h"

projectile::projectile()
{
	
}

projectile::projectile(sprite* p_sprite, SDL_Rect p_spawn_rect)
{
	m_sprite = p_sprite;

	m_x = p_spawn_rect.x;
	m_y = p_spawn_rect.y;
	m_w = p_spawn_rect.w;
	m_h = p_spawn_rect.h;

	m_collider = new rectangle_collider(m_x, m_y, m_w, m_h);

	set_active(false);

	m_type = ENTITY_TYPE::ENEMY;
}

projectile::~projectile()
{
}

void projectile::fire(int start_x, int start_y, int p_dir_x, int p_dir_y, int p_speed)
{
	m_x = start_x;
	m_y = start_y;
	m_dir_x = p_dir_x;
	m_dir_y = p_dir_y;
	m_speed = p_speed;

	m_collider->set_position(m_x, m_y);

	set_active(true);
}

void projectile::on_enable()
{
}

void projectile::update(int p_ms)
{
	// Cast the float position to the int position so it gets drawn in the right spot
	m_x += m_dir_x * m_speed;
	m_y += m_dir_y * m_speed;

	m_collider->set_position(m_x, m_y);
}

void projectile::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect());
}

void projectile::on_disable()
{
}

void projectile::on_collision(entity* p_other)
{
	set_active(false);	
}

void projectile::reset()
{
	set_active(false);
}
