// transform.h

#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include <vector>
#include "utilities/cyanide_math.h"

struct transform
{
	transform(const transform& p_transform);
	transform(transform* p_parent = nullptr,
			  const vector2& p_position = vector2::zero,
			  const vector2& p_scale = vector2::one,
			  const float& p_rotation = 0.0f);
	~transform();

	//	TODO: Add world matrix;
	//	set_world_position(vector2 p_position)
	//		m_position = p_position - m_parent->m_position;

	void set_local_position(vector2 p_position);
	void set_local_rotation(float p_rotation);
	void set_local_scale(vector2 p_scale);

	void set_world_position(vector2 p_position);
	void set_world_rotation(float p_rotation);
	void set_world_scale(vector2 p_scale);

	vector2 get_local_position();
	float get_local_rotation();
	vector2 get_local_scale();

	vector2 get_world_position();
	float get_world_rotation();
	vector2 get_world_scale();

	void set_parent(transform* p_parent);
	transform* get_parent();
	transform* get_child(unsigned int p_child_id);

private:
	vector2 m_position = vector2::zero;
	vector2 m_scale = vector2::one;
	float m_rotation = 0.0f;

	transform* m_parent = nullptr;
	std::vector<transform*> m_children;
};

#endif // !INCLUDED_TRANSFORM_H


