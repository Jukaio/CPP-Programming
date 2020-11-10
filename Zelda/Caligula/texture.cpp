// texture.cpp

#include "texture.h"
#include <SDL_image.h>

texture::texture(SDL_Texture* p_texture)
	: m_texture(p_texture)
{
}

texture::~texture()
{
	if(m_texture != nullptr)
		SDL_DestroyTexture(m_texture);
	m_texture = nullptr;
}

SDL_Texture* texture::get()
{
	return m_texture;
}
