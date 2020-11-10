// collider.cpp

#include "collider.h"

SDL_Rect rectangle_collider::get_bounds()
{
	return m_bounds;
}

rectangle_collider::rectangle_collider(int p_x, int p_y, int p_w, int p_h)
{
	m_type = COLLIDER_TYPE::RECTANGLE;
	m_bounds.x = p_x; m_bounds.y = p_y;
	m_bounds.w = p_w; m_bounds.h = p_h;
}

rectangle_collider::~rectangle_collider()
{
}

void collider::set_position(int p_x, int p_y)
{
	m_bounds.x = p_x; m_bounds.y = p_y;
}

void collider::set_bounds(const SDL_Rect& p_rect)
{
	m_bounds.x = p_rect.x;
	m_bounds.y = p_rect.y;
	m_bounds.w = p_rect.w;
	m_bounds.h = p_rect.h;
}


SDL_Rect circle_collider::get_bounds()
{
	return m_bounds;
}

circle_collider::circle_collider(int p_x, int p_y, float p_r)
{
	m_type = COLLIDER_TYPE::CIRCLE;
	m_bounds = { p_x, p_y, 0, 0 };
	m_radius = p_r;
}

circle_collider::~circle_collider()
{
}
