// lore_state.h

#ifndef INCLUDED_LORE_STATE_H
#define INCLUDED_LORE_STATE_H

#include "state.h"
#include <SDL_scancode.h>

struct lore_state : state
{
	lore_state(std::string p_name);
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private: 
	entity* m_last_page = nullptr;
};

#endif // !INCLUDED_LORE_STATE_H



