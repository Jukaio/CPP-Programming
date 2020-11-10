// text_map_format_atlas.h

#ifndef INCLUDED_TEXT_MAP_FORMAT_ATLAS_H
#define INCLUDED_TEXT_MAP_FORMAT_ATLAS_H

#include "resource_cache.h"
#include "text_map_format.h"

// stores all the possible maps, implying that all character maps a game uses are in the same source texture
// if variations of a character map exist through different texture files, then only one text_map_format is needed
// if the user of those classes has only one texture with all character maps, then more texture_map_formats are required
// I try to allow a little versatility for texture/data usage  

struct text_map_format_atlas
{
	bool load(std::string p_id, text_map_format* p_text_map_format);
	bool unload(std::string p_id);
	text_map_format* get(std::string p_id);
private:
	resource_cache<text_map_format> m_cache;
};

#endif // !INCLUDED_TEXT_MAP_FORMAT_ATLAS_H



