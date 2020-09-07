// maze.h

#ifndef INCLUDED_MAZE_H
#define INCLUDED_MAZE_H

#include <core/state.h>

static inline int random(int to)
{
	if(to == 0)
		return 0;
	return rand() % to;
}

static inline int random_direction()
{
	return -1 + ((rand() % 2) * 2);
}

struct maze_state : state
{
private:
	enum orientation
	{
		VERTICAL,
		HORIZONTAL
	};

	enum node_type
	{
		EMPTY = 0,
		OBSTACLE = 1 << 0,
		VISITED = 1 << 1,
	};

	struct node
	{
		char m_data;	// 0001 = Obstacle; 0010 = Visited
		float f;		// f = g + h (heuristic is a function) f is the full cost
		float g;		// g is the movement cost from start to node x
		vector2 m_position;
		std::vector<node*> m_neighbours; // very expensive, but
		node* m_parent;					 // allows easier implementation
	};

	struct maze // mate data 
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct grid
	{
		int cell_size;
		int w;
		int h;
	};

	maze m_maze;
	grid m_grid;

	void reset_maze();

	node* m_nodes = nullptr;
	node* m_node_start = nullptr;
	node* m_node_end = nullptr;
	std::vector<vector2> m_path;


	int choose_orientation(int width, int height);
	void divide_grid(int m_x, int m_y, int m_width, int m_height, int m_orientation);

	float heuristic(node* lhs, node* rhs);
	void a_star_path(std::vector<vector2>& p_path);

public:
	maze_state();

	virtual void enter(SDL_Renderer* p_renderer) override;
	virtual bool update(int p_ms) override;
	virtual bool late_update(int p_ms) override;
	virtual void render(SDL_Renderer* p_renderer) override;
	virtual void exit() override;

	
};

#endif // !INCLUDED_MAZE_H

