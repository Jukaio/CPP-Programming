// sdl_math.h

#ifndef INCLUDED_SDL_MATH_H
#define INCLUDED_SDL_MATH_H

#include <SDL_rect.h>

// Mostly SDL_Rect math which comes in handy for offsets and
// Rectangle multiplication 
// TODO: typedef Vector2 SDL_Point

inline SDL_Rect operator+(const SDL_Rect& lhs, const SDL_Rect& rhs)
{
	return { lhs.x + rhs.x,
			 lhs.y + rhs.y,
			 lhs.w + rhs.w,
			 lhs.h + rhs.h };
}

inline SDL_Rect operator-(const SDL_Rect& lhs, const SDL_Rect& rhs)
{
	return { lhs.x - rhs.x,
			 lhs.y - rhs.y,
			 lhs.w - rhs.w,
			 lhs.h - rhs.h };
}

inline SDL_Rect operator*(const SDL_Rect& lhs, const float factor)
{
	return { (int)(lhs.x * factor),
			 (int)(lhs.y * factor),
			 (int)(lhs.w * factor),
			 (int)(lhs.h * factor) };
}

inline SDL_Rect operator/(const SDL_Rect& lhs, const float factor)
{
	return { (int)(lhs.x / factor),
			 (int)(lhs.y / factor),
			 (int)(lhs.w / factor),
			 (int)(lhs.h / factor) };
}

inline SDL_Rect& operator+=(SDL_Rect& lhs, const SDL_Rect& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.w += rhs.w;
	lhs.h += rhs.h;
	return lhs;
}

inline SDL_Rect& operator-=(SDL_Rect& lhs, const SDL_Rect& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.w -= rhs.w;
	lhs.h -= rhs.h;
	return lhs;
}

inline SDL_Rect& operator*=(SDL_Rect& lhs, const float factor)
{
	(int)(lhs.x *= factor);
	(int)(lhs.y *= factor);
	(int)(lhs.w *= factor);
	(int)(lhs.h *= factor);
	return lhs;
}

inline SDL_Rect& operator/=(SDL_Rect& lhs, const float factor)
{
	(int)(lhs.x /= factor);
	(int)(lhs.y /= factor);
	(int)(lhs.w /= factor);
	(int)(lhs.h /= factor);
	return lhs;
}


#endif // !INCLUDED_SDL_MATH_H

