// lose_state.h

#ifndef INCLUDED_LOSE_STATE
#define INCLUDED_LOSE_STATE

#include "state.h"

struct lose_state : state
{
	lose_state(std::string p_name);
	~lose_state();
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private:
	float m_timer_duration;
	float m_timer;
};

#endif // !INCLUDED_LOSE_STATE



