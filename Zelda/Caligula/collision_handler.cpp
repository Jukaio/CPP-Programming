// collision_handler.cpp

#include "collision_handler.h"
#include "collider.h"

bool collision_handler::is_colliding(rectangle_collider * p_lhs, rectangle_collider * p_rhs)
{
	SDL_Rect left = p_lhs->get_bounds();
	SDL_Rect right = p_rhs->get_bounds();
	if (left.x > right.x + right.w ||
		left.x + left.w < right.x ||
		left.y > right.y + right.h ||
		left.y + left.h < right.y)
	{
		return false;
	}
	return true;
}

bool collision_handler::is_colliding(circle_collider* p_lhs, circle_collider* p_rhs)
{
	SDL_Rect left = p_lhs->get_bounds();
	SDL_Rect right = p_rhs->get_bounds();
	int x = left.x - right.x;
	int y = left.x - right.x;

	float distance = (float)sqrt(pow(x, 2) + pow(x, 2));
	if (distance <= p_lhs->m_radius + p_rhs->m_radius)
		return true;

	return false;
}

bool collision_handler::is_colliding(collider* p_lhs, collider* p_rhs)
{
	if (p_lhs->get_type() == COLLIDER_TYPE::RECTANGLE && p_rhs->get_type() == COLLIDER_TYPE::RECTANGLE)
	{
		return is_colliding(dynamic_cast<rectangle_collider*>(p_lhs), dynamic_cast<rectangle_collider*>(p_rhs));
	}
	else if(p_lhs->get_type() == COLLIDER_TYPE::CIRCLE && p_rhs->get_type() == COLLIDER_TYPE::CIRCLE)
	{
		return is_colliding(dynamic_cast<circle_collider*>(p_lhs), dynamic_cast<circle_collider*>(p_rhs));
	}
	else if(p_lhs->get_type() == COLLIDER_TYPE::RECTANGLE && p_rhs->get_type() == COLLIDER_TYPE::CIRCLE) // Not implemented!
	{
		return is_colliding(dynamic_cast<rectangle_collider*>(p_lhs), dynamic_cast<circle_collider*>(p_rhs));
	}
	else if(p_lhs->get_type() == COLLIDER_TYPE::CIRCLE && p_rhs->get_type() == COLLIDER_TYPE::RECTANGLE) // Not implemented!
	{
		return is_colliding(dynamic_cast<circle_collider*>(p_lhs), dynamic_cast<rectangle_collider*>(p_rhs));
	}
	return false;
}
