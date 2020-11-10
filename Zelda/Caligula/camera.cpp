// camera.cpp

#include "camera.h"
#include <SDL.h>

camera& camera::instance()
{
	static camera instance;
	return instance;
}

void camera::set_position(int p_x, int p_y)
{
	m_x = p_x;
	m_y = p_y;
}

SDL_Rect camera::get_position() const
{
	return { m_x, m_y, 0, 0 };
}

camera::camera()
{
}

