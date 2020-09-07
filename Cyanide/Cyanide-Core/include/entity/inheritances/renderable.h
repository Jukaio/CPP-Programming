// renderable.h

#ifndef INCLUDED_RENDERABLE_H
#define INCLUDED_RENDERABLE_H

#include "entity/data/transform.h"
#include <iostream>

struct SDL_Renderer;

struct renderable
{
	~renderable() = default;

	void set_visible(bool p_visible);
	bool get_visible();

	transform* get_transform();
	virtual void render(SDL_Renderer* p_renderer) = 0;

protected:
	renderable(vector2 p_position, 
			   float p_rotation, 
			   vector2 p_scale, 
			   transform* p_parent, 
			   bool p_visible);

	transform m_transform;

private:
	bool m_visible = false;
};

#endif // !INCLUDED_RENDERABLE_H



