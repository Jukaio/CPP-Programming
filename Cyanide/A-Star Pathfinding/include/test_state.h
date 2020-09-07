// test_state.h

#ifndef INCLUDED_TEST_STATE_H
#define INCLUDED_TEST_STATE_H

#include "core/state.h"
#include <iostream>

struct test_state : state
{
	test_state(int p_id);
	~test_state();

	// Inherited via state
	virtual void enter(SDL_Renderer* p_renderer) override;
	virtual bool update(int p_ms) override;
	virtual bool late_update(int p_ms) override;
	virtual void exit() override;

	// Inherited via state
	virtual void render(SDL_Renderer* p_renderer) override;

private:
	enum node_type 
	{
		NULL_TYPE = 0,
		OBSTACLE = 1 << 0,
		VISITED = 1 << 1,
	};
	struct node
	{
		char m_data;	// 0001 = Obstacle; 0010 = Visited
		float f;		// f = g + h (heuristic is a function) f is the full cost
		float g;		// g is the movement cost from start to node x
		vector2 m_position;
		std::vector<node*> m_neighbours;
		node* m_parent;
	};

	const int MAP_WIDTH		= 20;
	const int MAP_HEIGHT	= 20;
	const int CELL_SIZE		= 16;
	const int SCALE			= 2;

	node* m_nodes			= nullptr;

	node* m_node_start		= nullptr;
	node* m_node_end		= nullptr;

	std::vector<vector2> m_path;

	float heuristic(node* lhs, node* rhs);
	void a_star_path(std::vector<vector2>& out_path);

};

#endif // !INCLUDED_TEST_STATE_H
