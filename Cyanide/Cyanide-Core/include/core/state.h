// state.h

#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include <vector>
#include <iostream>

#include "entity/inheritances/collidable.h"
#include "entity/inheritances/updatable.h"
#include "entity/inheritances/renderable.h"
#include "entity/inheritances/destructable.h"


// base state, use enum to int cast for easier interface
struct state
{
private:
	enum base_type 
	{
		NO_BASE			= 0,
		COLLIDABLE		= 1 << 0,
		UPDATABLE		= 1 << 1,
		RENDERABLE		= 1 << 2,
		DESTRUCTABLE	= 1 << 3,
		INVALID_BASE	= 1 << 4, // Amount of relevant base types
	};

	template<typename T>
	int get_base_class_hex()
	{
		return	(COLLIDABLE		* std::is_base_of<collidable, T>::value)	+
				(UPDATABLE		* std::is_base_of<updatable, T>::value)		+
				(RENDERABLE		* std::is_base_of<renderable, T>::value)	+
				(DESTRUCTABLE	* std::is_base_of<destructable, T>::value); 
	}

public:
	state(int p_id)
		: m_id(p_id)
		, m_next(-1)
	{
	}
	virtual ~state()
	{
		for(auto&& e : m_destructables)
			delete e;
		m_destructables.clear();
		m_collidables.clear();
		m_updatables.clear();
		m_renderables.clear();
	}

	virtual void enter(SDL_Renderer* p_renderer) = 0;

	// Core update handles the update phases
	// The user can swap states through setting the next state 
	// and returning pre_update or post_update false
	virtual bool update(int p_ms) = 0;
	virtual bool late_update(int p_ms) = 0;
	virtual void render(SDL_Renderer* p_renderer) = 0;

	bool core_update(int p_ms)
	{
		bool pre = update(p_ms);
		update_entities(p_ms);
		bool post = late_update(p_ms);
		return pre && post;
	}
	void core_render(SDL_Renderer* p_renderer)
	{
		render_entities(p_renderer);
		render(p_renderer);
	}

	virtual void exit() = 0;

	int get_id()
	{
		return m_id;
	}
	void set_next(int p_id)
	{
		m_next = p_id;
	}
	int get_next()
	{
		return m_next;
	}

protected:
	// Doesn't really create an entity, rather passes a pointer of a created entity
	// and puts it into the entity collections
	// TODO: Change naming of create_entity functions
	template<typename T>
	void create_entity(T*& p_entity)
	{

		int hex_id = get_base_class_hex<T>();
		for(int id = 1; 
			id < INVALID_BASE || hex_id != 0; 
			id = id << 1)
		{
			switch(id & hex_id)
			{
				case NO_BASE:			/* Nothing really happens here */				break;
				case COLLIDABLE:	m_collidables.push_back((collidable*)p_entity);		break;
				case UPDATABLE:		m_updatables.push_back((updatable*)p_entity);		break;
				case RENDERABLE:	m_renderables.push_back((renderable*)p_entity);		break;
				case DESTRUCTABLE:	m_destructables.push_back((destructable*)p_entity); break;
			}
		}
		
	}
	template<typename T>
	T* create_entity(T*&& p_entity)
	{
		create_entity<T>(p_entity);
		return p_entity;
	}

	// Creates entity with passed parameters and returns it
	template<typename T, typename ... args>
	T* create_entity(args&&... p_args)
	{
		T* entity  = new T(std::forward<args>(p_args)...);

		create_entity<T>(entity);

		return entity;
	}

private: 
	int m_id;
	int m_next;

	std::vector<collidable*> m_collidables;
	std::vector<updatable*> m_updatables;
	std::vector<renderable*> m_renderables;
	std::vector<destructable*> m_destructables;

	void update_entities(int p_ms)
	{
		for(auto&& e : m_updatables)
		{
			if(e->get_active())
				e->update(p_ms);
		}
	}
	void render_entities(SDL_Renderer* p_renderer)
	{
		for(auto&& e : m_renderables)
		{
			if(e->get_visible())
				e->render(p_renderer);
		}
	}
};


#endif // !INCLUDED_STATE_H

