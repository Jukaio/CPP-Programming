// finite_state_machine.h

#ifndef INCLUDED_FINITE_STATE_MACHINE_H
#define INCLUDED_FINITE_STATE_MACHINE_H

#include <vector>
#include <string>
#include "resource_cache.h"

// The core of the game, but could also get modified and re-used for AI
// One big downside can be that I use strings, what motivates human error
// Otherwise it works 100% as intended -> Should use to enums for bigger projects

struct state;

struct finite_state_machine 
{
	finite_state_machine();
	~finite_state_machine();
	void update(int ms);
	void add_state(state* p_state);
	void remove_state(std::string p_state);
	void set_state(std::string p_state);

private:
	resource_cache<state> m_cache; // Collection of all states
	state* m_current_state;
	void switch_state(std::string p_state);
};

#endif // !INCLUDED_FINITE_STATE_MACHINE_H