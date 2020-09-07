// a_star.h

#ifndef INCLUDED_A_STAR_H
#define INCLUDED_A_STAR_H

#include "cyanide_core.h"

struct texture;

struct a_star : application
{
	a_star(int p_window_w, int p_window_h, float p_scale, const char* p_title);

	// Inherited via application
	virtual void application_enter() final;
	virtual void application_exit() final;
};



#endif // !INCLUDED_A_STAR_H
