// texture.h

#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

struct SDL_Texture;

struct texture 
{
	texture(SDL_Texture* p_texture);
	texture(const texture&) = default;
	texture(const texture* rhs);
	~texture();

	SDL_Texture* get();

private:
	SDL_Texture* m_texture; 
};

#endif // !INCLUDED_TEXTURE_H



