// collision_handler.h

#ifndef INCLUDED_COLLISION_HANDLER_H
#define INCLUDED_COLLISION_HANDLER_H

struct collider;
struct rectangle_collider;
struct circle_collider;

// instead of making static functions, I chose to make memmber functions,
// because it allows a future implementation of more complex "collision handlers"
// which allow enter, exit and stay behaviour

struct collision_handler
{
	// Wrapper for collision 
	bool is_colliding(collider* p_lhs, collider* p_rhs); 

private:

	// Specific collision implementations
	bool is_colliding(rectangle_collider* p_lhs, rectangle_collider* p_rhs);
	bool is_colliding(circle_collider* p_lhs, circle_collider* p_rhs);
};

#endif // !INCLUDED_COLLISION_HANDLER_H

