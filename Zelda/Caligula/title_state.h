// title_state.h

#ifndef INCLUDED_TITLE_STATE_H
#define INCLUDED_TITLE_STATE_H

#include "state.h"
#include <SDL_scancode.h>

struct title_state : state
{
	title_state(std::string p_name);
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private:
	float m_timer;
};

#endif // !INCLUDED_TITLE_STATE_H



