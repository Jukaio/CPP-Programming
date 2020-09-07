// collider.h

#ifndef INCLUDED_COLLIDER_H
#define INCLUDED_COLLIDER_H

#include "entity/data/transform.h"

struct collider
{
	collider(transform* p_entity_transform);

private:
	transform m_transform;
	transform* m_entity_transform;
};

#endif // !INCLUDED_COLLIDER_H



