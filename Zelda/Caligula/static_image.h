// static_image.h

#ifndef INCLUDED_STATIC_IMAGE_H
#define INCLUDED_STATIC_IMAGE_H

#include "entity.h"
#include "config.h"

// A class which gives a sprite also a destination 
// The destination is important so it can get drawn

struct SDL_Rect;

struct static_image : entity
{
	static_image(sprite* p_sprite, SDL_Rect p_destination, int p_scale = CONFIG::SCALE);
	
	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;
};

#endif // !INCLUDED_STATIC_IMAGE_H



