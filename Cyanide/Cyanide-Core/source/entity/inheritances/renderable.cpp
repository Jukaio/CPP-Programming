// renderable.cpp

#include "entity/inheritances/renderable.h"
#include "entity/data/transform.h"

renderable::renderable(vector2 p_position, float p_rotation, vector2 p_scale, transform* p_parent, bool p_visible)
    : m_visible(p_visible)
    , m_transform(p_parent, p_position, p_scale, p_rotation)
{
    
}

void renderable::set_visible(bool p_visible)
{
    m_visible = p_visible;   
}

bool renderable::get_visible()
{
    return m_visible;
}

transform* renderable::get_transform()
{
    return &m_transform;
}
