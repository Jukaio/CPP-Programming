// texture.h

#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

// wrapper class for SDL_Texture to ensure correct destruction when the resource_cache gets used
// the destructor calls the DestroyTexture function instead of using the delete operator

struct SDL_Texture;

struct texture
{
	texture(SDL_Texture* p_texture);
	~texture();

	SDL_Texture* get();

private:
	SDL_Texture* m_texture = nullptr;
};

#endif // !INCLUDED_TEXTURE_H



