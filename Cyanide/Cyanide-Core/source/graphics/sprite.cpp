// sprite.cpp

#include "graphics/texture.h"
#include "graphics/sprite.h"

sprite::sprite(texture* p_texture, 
			   const rectangle& p_rect, 
			   const vector2& p_relative_origin,
			   sprite_flip p_flip)
	: m_texture(p_texture)
	, m_rect(p_rect)
	, m_origin({ (int)(p_relative_origin.x * p_rect.w), 
				 (int)(p_relative_origin.y * p_rect.h)})
	, m_flip(p_flip)
{

}

sprite::~sprite()
{

}

texture* sprite::get_texture()
{
	return m_texture;
}

rectangle* sprite::get_rect()
{
	return &m_rect;
}

vector2* sprite::get_origin()
{
	return &m_origin;
}

sprite_flip sprite::get_flip()
{
	return m_flip;
}
