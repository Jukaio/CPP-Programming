// char.h

#ifndef INCLUDED_CHARACTER_H
#define INCLUDED_CHARACTER_H

// source data of a character
// basiccally just a wrapper class of SDL_Rect
// Could have used a typedef, but serves the same purpose

struct SDL_Rect;

struct character
{
	character(SDL_Rect p_source);
	SDL_Rect get_rect();

private:
	int m_x, m_y = 0; 
	int m_w, m_h = 0;
};

#endif // !INCLUDED_CHARACTER_H



