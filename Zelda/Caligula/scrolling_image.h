// scrolling_image.h

#ifndef INCLUDED_SCROLLING_IMAGE_H
#define INCLUDED_SCROLLING_IMAGE_H

#include "static_image.h"
#include "config.h"

// A static image but with added movement behaviour 

struct scrolling_image : static_image
{
	scrolling_image(sprite* p_sprite, SDL_Rect p_destination, int p_speed_x, int p_speed_y, int p_scale = CONFIG::SCALE);
	virtual void update(int p_ms) override;

private:
	int m_speed_x;
	int m_speed_y;
};

#endif // !INCLUDED_SCROLLING_IMAGE_H



