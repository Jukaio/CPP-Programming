// sprite.cpp

#include "sprite.h"
#include <SDL_image.h>

sprite::sprite(SDL_Texture* p_texture, SDL_Rect p_rect)
	: m_texture(p_texture)
	, m_x(p_rect.x)
	, m_y(p_rect.y)
	, m_w(p_rect.w)
	, m_h(p_rect.h)
{
	
}

sprite::~sprite()
{
	m_texture = nullptr;
}

SDL_Texture* sprite::get_texture()
{
	return m_texture;
}

SDL_Rect sprite::get_rect()
{
	return {m_x, m_y, m_w, m_h};
}
