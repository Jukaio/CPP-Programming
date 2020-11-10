// text.cpp

#include "text.h"
#include "text_map_format.h"
#include <SDL_image.h>

text::text(std::string p_text, texture* p_texture, text_map_format* p_text_map_format)
	: m_string(p_text)
	, m_texture(p_texture)
	, m_text_map_format(p_text_map_format)
{
}

text::~text()
{
	m_texture = nullptr;
	m_text_map_format = nullptr;
}

void text::set_string(std::string p_string)
{
	m_string = p_string;
}

std::string* text::get_string()
{
	return &m_string;
}

texture* text::get_texture()
{
	return m_texture;
}

text_map_format* text::get_text_map_format()
{
	return m_text_map_format;
}

