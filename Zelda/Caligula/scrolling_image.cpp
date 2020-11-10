// scrolling_image.cpp

#include "scrolling_image.h"
#include <SDL_rect.h>

scrolling_image::scrolling_image(sprite* p_sprite, 
								 SDL_Rect p_destination, 
								 int p_speed_x,
								 int p_speed_y,
								 int p_scale)
	: static_image(p_sprite, p_destination, p_scale)
	, m_speed_x(p_speed_x)
	, m_speed_y(p_speed_y)
{

}

void scrolling_image::update(int p_ms)
{
	m_x += m_speed_x;
	m_y += m_speed_y;
}
