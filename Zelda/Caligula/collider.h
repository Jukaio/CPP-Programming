// collider.h

#ifndef INCLUDED_COLLIDER_H
#define INCLUDED_COLLIDER_H

#include <SDL.h>

// Based on the type, the collider is used for the collision function
// Each collider gets "checked" differently
// Rectangle gets checked through position, whidth and height
// Circle gets checked through position and radius

enum COLLIDER_TYPE
{
	RECTANGLE,
	CIRCLE
};

struct collider
{
	// Transform collider
	// Position, shape, etc.
	void set_position(int p_x, int p_y);
	void set_bounds(const SDL_Rect& p_rect);
	virtual SDL_Rect get_bounds() = 0;

	// active component
	bool is_active() { return m_active; }
	void set_active(bool p_active) { m_active = p_active; };

	COLLIDER_TYPE get_type() { return m_type; }
	collider() {}
	virtual ~collider() {};

protected: 
	SDL_Rect m_bounds;
	COLLIDER_TYPE m_type;
	bool m_active = false;
};

struct rectangle_collider : public collider
{
	SDL_Rect get_bounds();
	rectangle_collider(int p_x, int p_y, int p_w, int p_h);
	~rectangle_collider();
};

struct circle_collider : public collider
{
	SDL_Rect get_bounds();
	float m_radius;
	circle_collider(int p_x, int p_y, float p_r);
	~circle_collider();
};

#endif // !INCLUDED_COLLIDER_H