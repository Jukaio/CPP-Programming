// texture.cpp

#include "graphics/texture.h"
#include <iostream>
#include <SDL_render.h>

texture::texture(SDL_Texture* p_texture)
{
    std::cout << "Create texture \n";
    m_texture = p_texture;
}

texture::texture(const texture* rhs)
{
    *this = *rhs;
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

