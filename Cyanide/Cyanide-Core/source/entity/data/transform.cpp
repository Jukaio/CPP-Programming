// transform.cpp

#include <assert.h>
#include <iostream>
#include "entity/data/transform.h"

transform::transform(const transform& p_transform)
{
	m_position	= p_transform.m_position;
	m_scale		= p_transform.m_scale;
	m_rotation	= p_transform.m_rotation;
}

transform::transform(transform* p_parent,
					 const vector2& p_position, 
					 const vector2& p_scale, 
					 const float& p_rotation)
{
	if(p_parent)
		set_parent(p_parent);
	m_position	= p_position;
	m_scale		= p_scale;
	m_rotation	= p_rotation;
}

transform::~transform()
{
	m_children.clear();
}

void transform::set_local_position(vector2 p_position)	{ m_position = p_position; }
void transform::set_local_rotation(float p_rotation)	{ m_rotation = p_rotation; }
void transform::set_local_scale(vector2 p_scale)		{ m_scale = p_scale; }

void transform::set_world_position(vector2 p_position)	{ m_position = m_parent ? p_position - m_parent->get_world_position() : p_position; }
void transform::set_world_rotation(float p_rotation)	{ m_rotation = m_parent ? p_rotation - m_parent->get_world_rotation() : p_rotation; }
void transform::set_world_scale(vector2 p_scale)		{ m_scale = m_parent ? p_scale / m_parent->get_world_scale(): p_scale; }

vector2 transform::get_local_position()					{ return m_position; }
float transform::get_local_rotation()					{ return m_rotation; }
vector2 transform::get_local_scale()					{ return m_scale; }

vector2 transform::get_world_position()					{ return m_parent ? m_position + m_parent->get_world_position() : m_position; }
float transform::get_world_rotation()					{ return m_parent ? m_rotation + m_parent->get_world_rotation() : m_rotation; }
vector2 transform::get_world_scale()		            { return m_parent ? m_scale * m_parent->get_world_scale() : m_scale; }


void transform::set_parent(transform* p_parent)
{
	p_parent->m_children.push_back(this);
	m_parent = p_parent;
}

transform* transform::get_parent()
{
	return m_parent;
}

transform* transform::get_child(unsigned int p_child_id)
{
	if(p_child_id < m_children.size())
		return m_children[p_child_id];
	assert(!"Error:no child in index\n");
	std::cout << "No child in index!\n";
	return nullptr;
}
