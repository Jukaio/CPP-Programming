// maze_state.cpp

#include "maze.h"

#include "utilities/service.h"
#include "input/input_handler.h"
#include <SDL.h>

#include <list>
#include <chrono>

maze_state::maze_state()
	: state(0)
{
	std::cout << "Cell size: ";
	std::cin >> m_grid.cell_size;

	auto input = service<input_handler>::get();
	auto window_size = input->get_window_size();
	m_grid.w = window_size.x;
	m_grid.h = window_size.y;

	m_grid.w /= m_grid.cell_size;
	m_grid.h /= m_grid.cell_size;

	m_nodes = new node[m_grid.w * m_grid.h];
	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			m_nodes[y * m_grid.w + x].m_position = { (float)x, (float)y };
			m_nodes[y * m_grid.w + x].m_data = 0;
		}

	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			if(y > 0)				m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y - 1) * m_grid.w + (x + 0)]);
			if(y < m_grid.h - 1)	m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 1) * m_grid.w + (x + 0)]);
			if(x > 0)				m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 0) * m_grid.w + (x - 1)]);
			if(x < m_grid.w - 1)	m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 0) * m_grid.w + (x + 1)]);
		}

	m_maze.x = 1;
	m_maze.y = 1;
	m_maze.w = m_grid.w - 2;
	m_maze.h = m_grid.h - 2;
	bool double_row_x = !(m_maze.w % 2);
	bool double_row_y = !(m_maze.h % 2);

	auto now = std::chrono::system_clock::now().time_since_epoch();
	long long seed = now.count();
	srand(seed);

	divide_grid(m_maze.x, m_maze.y, m_maze.w, m_maze.h, choose_orientation(m_grid.w, m_grid.h));

	for(int i = 0; i < m_grid.w; i++)
	{
		int index = 0 * m_grid.w + i;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	for(int i = 0; i < m_grid.h; i++)
	{
		int index = i * m_grid.w + 0;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}

	for(int i = 0; i < m_grid.w; i++)
	{
		int index = (m_grid.h - 1) * m_grid.w + i;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	if(double_row_x)
		for(int i = 0; i < m_grid.w; i++)
		{
			int index = (m_grid.h - 2) * m_grid.w + i;
			node* temp = &m_nodes[index];
			temp->m_data = OBSTACLE;
		}

	for(int i = 0; i < m_grid.h; i++)
	{
		int index = i * m_grid.w + (m_grid.w - 1);
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	if(double_row_y)
		for(int i = 0; i < m_grid.h; i++)
		{
			int index = i * m_grid.w + (m_grid.w - 2);
			node* temp = &m_nodes[index];
			temp->m_data = OBSTACLE;
		}

	// Start Room
	for(int x = 1; x < 6; x++)
		for(int y = 1; y < 6; y++)
		{
			m_nodes[y * m_grid.w + x].m_data = 0;
		}

	// End Room
	// Some offset values
	int draw_start = (6 + double_row_x);
	int draw_end = (1 + double_row_x);
	for(int x = m_grid.w - draw_start; x < m_grid.w - draw_end; x++)
		for(int y = m_grid.h - draw_start; y < m_grid.h - draw_end; y++)
		{
			m_nodes[y * m_grid.w + x].m_data = 0;
		}

	m_node_start = &m_nodes[3 * m_grid.w + 3];
	m_node_end = &m_nodes[(m_grid.h - (4 + double_row_y)) * m_grid.w + (m_grid.w - (4 + double_row_x))];

	a_star_path(m_path);
}

void maze_state::enter(SDL_Renderer* p_renderer)
{
}
bool maze_state::update(int p_ms)
{
	const int total = m_grid.cell_size;
	auto input = service<input_handler>::get();

	if(input->window_resized())
	{ 
		reset_maze();
	}

	if(input->is_down(mouse::button::LEFT))
	{
		int y = (int)input->mouse_position().y / total;
		int x = (int)input->mouse_position().x / total;
		
		node* temp = &m_nodes[y * m_grid.w + x];

		if(input->is_down(keyboard::key::LCTRL))
		{
			if(!(temp->m_data & OBSTACLE))
				m_node_start = temp;
		}
		else if(input->is_down(keyboard::key::LSHIFT))
		{
			if(!(temp->m_data & OBSTACLE)) 
				m_node_end = temp;
		}
		a_star_path(m_path);
	}

	return true;
}
bool maze_state::late_update(int p_ms)
{
	return true;
}
void maze_state::render(SDL_Renderer* p_renderer)
{
	int window_w;
	int window_h;
	auto input = service<input_handler>::get();
	auto window_size = input->get_window_size();
	window_w = window_size.x;
	window_h = window_size.y;

	SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, 255);
	SDL_Rect background = { 0, 0,
							window_w, window_h };
	SDL_RenderFillRect(p_renderer, &background);

	SDL_SetRenderDrawColor(p_renderer, 11, 11, 22, 255);
	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			node* n = &m_nodes[y * m_grid.w + x];
			for(auto&& c : n->m_neighbours)
				SDL_RenderDrawLine(p_renderer,
								   n->m_position.x * m_grid.cell_size + m_grid.cell_size / 2,
								   n->m_position.y * m_grid.cell_size + m_grid.cell_size / 2,
								   c->m_position.x * m_grid.cell_size + m_grid.cell_size / 2,
								   c->m_position.y * m_grid.cell_size + m_grid.cell_size / 2);
		}

	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			node* n = &m_nodes[y * m_grid.w + x];
			SDL_Rect rect = { (int)n->m_position.x * m_grid.cell_size,
							  (int)n->m_position.y * m_grid.cell_size,
							  m_grid.cell_size, m_grid.cell_size };

			if(n == m_node_start)			SDL_SetRenderDrawColor(p_renderer, 0, 255, 0, 255);
			else if(n == m_node_end)		SDL_SetRenderDrawColor(p_renderer, 255, 0, 0, 255);
			else if(n->m_data & OBSTACLE)	SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, 255);
			//else if(n->m_data & VISITED)	SDL_SetRenderDrawColor(p_renderer, 75, 25, 125, 255); // Draw A-Star visited
			else							SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);

			SDL_RenderFillRect(p_renderer, &rect);
		}

	for(size_t i = 0; i < m_path.size() - 1; i++)
	{
		int red = 255 - (int)(255 * ((i + 1.0f) / m_path.size()));
		int blue = (int)(255 * ((i + 1.0f) / m_path.size()));
		SDL_SetRenderDrawColor(p_renderer, red, blue, 255, 255);
		auto current = &m_path[i];
		auto next = &m_path[i + 1];
		SDL_RenderDrawLine(p_renderer,
						   current->x * m_grid.cell_size + m_grid.cell_size / 2,
						   current->y * m_grid.cell_size + m_grid.cell_size / 2,
						   next->x * m_grid.cell_size + m_grid.cell_size / 2,
						   next->y * m_grid.cell_size + m_grid.cell_size / 2);
	}
}
void maze_state::exit()
{
	delete[] m_nodes;
}

void maze_state::reset_maze()
{
	delete[] m_nodes;

	// create grid
	auto input = service<input_handler>::get();
	auto window_size = input->get_window_size();
	m_grid.w = window_size.x;
	m_grid.h = window_size.y;

	m_grid.w /= m_grid.cell_size;
	m_grid.h /= m_grid.cell_size;

	m_nodes = new node[m_grid.w * m_grid.h];
	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			m_nodes[y * m_grid.w + x].m_position = { (float)x, (float)y };
			m_nodes[y * m_grid.w + x].m_data = 0;
		}


	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			if(y > 0)				m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y - 1) * m_grid.w + (x + 0)]);
			if(y < m_grid.h - 1)	m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 1) * m_grid.w + (x + 0)]);
			if(x > 0)				m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 0) * m_grid.w + (x - 1)]);
			if(x < m_grid.w - 1)	m_nodes[y * m_grid.w + x].m_neighbours.push_back(&m_nodes[(y + 0) * m_grid.w + (x + 1)]);
		}

	// maze-to-map offsets
	m_maze.x = 1;
	m_maze.y = 1;
	m_maze.w = m_grid.w - 2;
	m_maze.h = m_grid.h - 2;
	bool double_row_x = !(m_maze.w % 2);
	bool double_row_y = !(m_maze.h % 2);

	divide_grid(m_maze.x, m_maze.y, m_maze.w, m_maze.h, choose_orientation(m_grid.w, m_grid.h));


	for(int i = 0; i < m_grid.w; i++)
	{
		int index = 0 * m_grid.w + i;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	for(int i = 0; i < m_grid.h; i++)
	{
		int index = i * m_grid.w + 0;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}

	for(int i = 0; i < m_grid.w; i++)
	{
		int index = (m_grid.h - 1) * m_grid.w + i;
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	if(double_row_y)
		for(int i = 0; i < m_grid.w; i++)
		{
			int index = (m_grid.h - 2) * m_grid.w + i;
			node* temp = &m_nodes[index];
			temp->m_data = OBSTACLE;
		}

	for(int i = 0; i < m_grid.h; i++)
	{
		int index = i * m_grid.w + (m_grid.w - 1);
		node* temp = &m_nodes[index];
		temp->m_data = OBSTACLE;
	}
	if(double_row_x)
		for(int i = 0; i < m_grid.h; i++)
		{
			int index = i * m_grid.w + (m_grid.w - 2);
			node* temp = &m_nodes[index];
			temp->m_data = OBSTACLE;
		}

	// Start Room
	for(int x = 1; x < 6; x++)
		for(int y = 1; y < 6; y++)
		{
			m_nodes[y * m_grid.w + x].m_data = 0;
		}

	// End Room
	// Some offset values
	int draw_start_x = (6 + double_row_x);
	int draw_end_x = (1 + double_row_x);
	int draw_start_y = (6 + double_row_y);
	int draw_end_y = (1 + double_row_y);
	for(int x = m_grid.w - draw_start_x; x < m_grid.w - draw_end_x; x++)
		for(int y = m_grid.h - draw_start_y; y < m_grid.h - draw_end_y; y++)
		{
			m_nodes[y * m_grid.w + x].m_data = 0;
		}

	m_node_start = &m_nodes[3 * m_grid.w + 3];
	m_node_end = &m_nodes[(m_grid.h - (4 + double_row_y)) * m_grid.w + (m_grid.w - (4 + double_row_x))];

	a_star_path(m_path);
}
int maze_state::choose_orientation(int width, int height)
{
	return ((width < height) * HORIZONTAL) + 
		   ((width == height) * rand() % 1);
}
void maze_state::divide_grid(int p_x, int p_y, int p_width, int p_height, int p_orientation)
{
	if (p_width < 2 || p_height < 2)
		return;

	int dx = (p_orientation == HORIZONTAL);
	int dy = (p_orientation == VERTICAL);

	int rx = p_x + (dy * random(p_width - 2));
	int ry = p_y + (dx * random(p_height - 2));

	rx += dy * (rx % 2);
	ry += dx * (ry % 2);

	int px = rx + (dx * random(p_width));
	int py = ry + (dy * random(p_height));

	if(dx && !(px % 2))
	{
		if(px <= m_maze.x)		px++;
		else if(px >= m_maze.w) px--;
		else					px += random_direction();
	}
	else if(dy && !(py % 2))
	{
		if(py <= m_maze.y)		py++;
		else if(py >= m_maze.h)	py--;
		else					py += random_direction();
	}

	int length = dx * p_width + dy * p_height;

	for(int i = 0; i < length; i++)
	{
		int x = rx + (dx * i);
		int y = ry + (dy * i);

		m_nodes[y * m_grid.w + x].m_data = OBSTACLE * !(px == x && py == y);
	}

	int nx = p_x;
	int ny = p_y;
	int w = (dx * p_width) + (dy * (rx - p_x));
	int h = (dy * p_height) + (dx * (ry - p_y));
	divide_grid(nx, ny, w, h, choose_orientation(w, h));

	nx = (dx * p_x) + (dy * (rx + 1));
	ny = (dy * p_y) + (dx * (ry + 1));
	w = (dx * p_width) + (dy * (p_x + p_width - rx - 1));
	h = (dy * p_height) + (dx * (p_y + p_height- ry - 1));
	divide_grid(nx, ny, w, h, choose_orientation(w, h));
}
float maze_state::heuristic(node* lhs, node* rhs)
{
	return vector2::distance(lhs->m_position, rhs->m_position);
}
void maze_state::a_star_path(std::vector<vector2>& out_path)
{
	// Reset the grid and put the values on their "defaults"
	for(int x = 0; x < m_grid.w; x++)
		for(int y = 0; y < m_grid.h; y++)
		{
			m_nodes[y * m_grid.w + x].m_data = m_nodes[y * m_grid.w + x].m_data & OBSTACLE;
			m_nodes[y * m_grid.w + x].f = INFINITY;
			m_nodes[y * m_grid.w + x].g = INFINITY;
			m_nodes[y * m_grid.w + x].m_parent = nullptr;
		}

	// Prepare the starting point
	node* current = m_node_start;
	m_node_start->g = 0.0f;
	m_node_start->f = heuristic(m_node_start, m_node_end);

	std::list<node*> open_nodes;
	open_nodes.push_back(m_node_start);

	while(!open_nodes.empty() && current != m_node_end)
	{
		open_nodes.sort([](const node* lhs, const node* rhs)
						{
							return lhs->f < rhs->f;
						});

		while(!open_nodes.empty() && open_nodes.front()->m_data & node_type::VISITED)
			open_nodes.pop_front();

		if(open_nodes.empty())
			break;

		current = open_nodes.front();
		current->m_data += node_type::VISITED;

		for(auto&& neighbour : current->m_neighbours)
		{
			if(!(neighbour->m_data & node_type::VISITED) &&
			   !(neighbour->m_data & node_type::OBSTACLE))
				open_nodes.push_back(neighbour);

			// if the movement cost g from the current one plus the target neighbour distace is smaller than
			// neighbours movement cost -> add current to the path
			float lower_cost = current->g + vector2::distance(current->m_position, neighbour->m_position);
			if(lower_cost < neighbour->g)
			{
				neighbour->m_parent = current;
				neighbour->g = lower_cost;
				neighbour->f = neighbour->g + heuristic(neighbour, m_node_end);
			}
		}
	}

	out_path.clear();
	node* temp = m_node_end;
	while(temp->m_parent != nullptr)
	{
		out_path.push_back(temp->m_position);
		temp = temp->m_parent;
	}
	out_path.push_back(m_node_start->m_position);
}
