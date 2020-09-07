// FSM.h

#ifndef INCLUDED_FSM_H
#define INCLUDED_FSM_H

#include "utilities/resource_cache.h"

struct state;
struct SDL_Renderer;

struct FSM
{
	FSM();
	~FSM();

	void set_renderer(SDL_Renderer* p_renderer);

	void run(int p_ms);
	void add_state(state* p_state);
	void remove_state(int p_id);
	void set_state(int p_id);
	void shut_down();

private:
	SDL_Renderer* m_renderer = nullptr;
	state* m_current = nullptr;
	resource_cache<int, state> m_cache;

};

#endif // !INCLUDED_FSM_H

