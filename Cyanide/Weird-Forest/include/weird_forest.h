// weird_forest.h

#ifndef INCLUDED_WEIRD_FOREST_H
#define INCLUDED_WEIRD_FOREST_H

#include <core/application.h>

struct weird_forest : application
{
	weird_forest(int p_window_w, int p_window_h, float p_scale, const char* p_title);
	
	// Inherited via application
	virtual void application_enter() override;
	virtual void application_exit() override;
};

#endif // !INCLUDED_WEIRD_FOREST_H
