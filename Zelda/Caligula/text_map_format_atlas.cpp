// text_map_format_atlas.cpp

#include "text_map_format_atlas.h"

bool text_map_format_atlas::load(std::string p_id, text_map_format* p_text_map_format)
{
	if(p_text_map_format == nullptr)
		return false;

	m_cache.add(p_id, p_text_map_format);
	return true;
}

bool text_map_format_atlas::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

text_map_format* text_map_format_atlas::get(std::string p_id)
{
	return m_cache[p_id];
}
