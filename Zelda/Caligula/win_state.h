// win_state.h

#ifndef INCLUDED_WIN_STATE
#define INCLUDED_WIN_STATE

#include "state.h"

struct win_state : state
{
	win_state(std::string p_name);
	~win_state();
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private:
	int m_timer_duration;
	int m_timer;
};

#endif // !INCLUDED_WIN_STATE

