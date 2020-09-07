// sprite.h

#ifndef INCLUDED_SPRITE_H
#define INCLUDED_SPRITE_H

#include "utilities/cyanide_math.h"

enum class sprite_flip
{
	NONE		= 0x00000000, // do not flip 
	HORIZONTAL  = 0x00000001, // flip horizontally 
	VERTICAL	= 0x00000002  // flip vertically 
};

struct texture;

struct sprite
{
	sprite(texture* p_texture, 
		   const rectangle& p_rect,
		   const vector2& p_relative_origin = {0.5f, 0.5f},
		   sprite_flip p_flip = sprite_flip::NONE);
	~sprite();

	texture* get_texture();
	rectangle* get_rect();
	vector2* get_origin();
	sprite_flip get_flip();

private:
	texture* m_texture;
	rectangle m_rect;
	vector2 m_origin;
	sprite_flip m_flip;
};

#endif // !INCLUDED_SPRITE_H



