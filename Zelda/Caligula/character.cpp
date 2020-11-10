// char.cpp

#include "character.h"
#include <SDL_image.h>

character::character(SDL_Rect p_rect)
	: m_x(p_rect.x)
	, m_y(p_rect.y)
	, m_w(p_rect.w)
	, m_h(p_rect.h)
{
}

SDL_Rect character::get_rect()
{
	return {m_x, m_y, m_w, m_h};
}
