// zelda.h

#ifndef INCLUDED_ZELDA_H
#define INCLUDED_ZELDA_H

#include "game.h"

struct zelda : game
{
	zelda(SDL_Renderer* p_renderer);
	
	// Final keywords, since this is supposed to be the final implementation of game
	virtual void enter() final;
	virtual void run() final;
	virtual void exit() final;
};

#endif // !INCLUDED_ZELDA_H


