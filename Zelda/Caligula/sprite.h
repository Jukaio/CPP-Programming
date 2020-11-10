// sprite.h

#ifndef INCLUDED_SPRITE_H
#define INCLUDED_SPRITE_H

// source rectangle + texture = sprite
// A source rectangle describes an area on the texture


struct SDL_Rect;
struct SDL_Texture;

struct sprite
{
	sprite(SDL_Texture* p_texture, SDL_Rect p_rect);
	~sprite();
	SDL_Texture* get_texture();
	SDL_Rect get_rect();

private:
	SDL_Texture* m_texture = nullptr;
	int m_x, m_y = 0;
	int m_w, m_h = 0;
};

#endif // !INCLUDED_SPRITE_H



