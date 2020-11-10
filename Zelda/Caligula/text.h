// text.h

#ifndef INCLUDED_TEXT_H
#define INCLUDED_TEXT_H

#include "character.h"
#include <vector>
#include <string>

// uses a string to describe what to write, while the texture and text_map_format decide the visual appearance
// text doesn't use a font, it uses bitmaps (or just plain textures)
// text_map_format describes how to read the source texture, while the string describes the word (position in the texture)

struct texture;
struct SDL_Rect;
struct text_map_format;

struct text
{
	text(std::string p_text, texture* p_texture, text_map_format* p_text_map_format);
	~text();

	void set_string(std::string p_string);
	std::string* get_string();

	texture* get_texture();
	text_map_format* get_text_map_format();

private:
	texture* m_texture = nullptr;
	std::string m_string;
	text_map_format* m_text_map_format = nullptr;
};

#endif // !INCLUDED_TEXT_H



