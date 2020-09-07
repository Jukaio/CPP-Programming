// collidable.h

#ifndef INCLUDED_COLLIDABLE_H
#define INCLUDED_COLLIDABLE_H

struct collider;

struct collidable
{
	~collidable() = default;

protected:
	collidable(collider* p_collider);

	virtual void on_collision_enter(collidable* p_other) = 0;
	virtual void on_collision_stay(collidable* p_other) = 0;
	virtual void on_collision_exit(collidable* p_other) = 0;

protected:
	collider* m_collider;
};

#endif // !INCLUDED_COLLIDABLE_H



