// test_state.cpp

#include "test_state.h"
#include "utilities/service.h"
#include "graphics/texture_manager.h"
#include "test_entity.h"

#include "graphics/sprite.h"

#include "utilities/service.h"
#include "input/input_handler.h"

#include <SDL.h>
#include <list>

test_state::test_state(int p_id)
	:state(p_id)
{
	m_nodes = new node[MAP_WIDTH * MAP_HEIGHT];
	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			m_nodes[y * MAP_WIDTH + x].m_position = { (float)x, (float)y };
			m_nodes[y * MAP_WIDTH + x].m_parent = nullptr;
			m_nodes[y * MAP_WIDTH + x].m_data = 0;
		}

	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			if(y > 0)									m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y - 1) * MAP_WIDTH + (x + 0)]);
			if(y < MAP_HEIGHT - 1)						m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y + 1) * MAP_WIDTH + (x + 0)]);
			if(x > 0)									m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y + 0) * MAP_WIDTH + (x - 1)]);
			if(x < MAP_WIDTH - 1)						m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y + 0) * MAP_WIDTH + (x + 1)]);

			if(y > 0 && x > 0)							m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y - 1) * MAP_WIDTH + (x - 1)]);
			if(y < MAP_HEIGHT - 1 && x > 0)				m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y + 1) * MAP_WIDTH + (x - 1)]);
			if(y > 0 && x < MAP_WIDTH - 1)				m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y - 1) * MAP_WIDTH + (x + 1)]);
			if(y < MAP_HEIGHT - 1 && x < MAP_WIDTH - 1)	m_nodes[y * MAP_WIDTH + x].m_neighbours.push_back(&m_nodes[(y + 1) * MAP_WIDTH + (x + 1)]);
		}

	m_node_start = &m_nodes[MAP_HEIGHT / 2 * MAP_WIDTH + 2];
	m_node_end = &m_nodes[MAP_HEIGHT / 2 * MAP_WIDTH + MAP_WIDTH - 2];

	a_star_path(m_path);
}

test_state::~test_state()
{

}

void test_state::enter(SDL_Renderer* p_renderer)
{
	SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
}

bool test_state::update(int p_ms)
{
	const int total = CELL_SIZE * SCALE;
	auto input = service<input_handler>::get();
	if(input->is_down(mouse::button::LEFT))
	{
		int y = (int)input->mouse_position().y / total;
		int x = (int)input->mouse_position().x / total;
		
		node* temp = &m_nodes[y * MAP_WIDTH + x];

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
		else
		{
			if(temp != m_node_start &&
			   temp != m_node_end &&
			   !(temp->m_data & OBSTACLE))
				temp->m_data += OBSTACLE;
		}
		a_star_path(m_path);
	}
	else if(input->is_down(mouse::button::RIGHT))
	{
		int y = (int)input->mouse_position().y / total;
		int x = (int)input->mouse_position().x / total;

		node* temp = &m_nodes[y * MAP_WIDTH + x];

		if(temp != m_node_start &&
		   temp != m_node_end &&
		   temp->m_data & OBSTACLE)
			temp->m_data -= OBSTACLE;

		a_star_path(m_path);
	}


	return true;
}

bool test_state::late_update(int p_ms)
{
	
	return true;
}

void test_state::render(SDL_Renderer* p_renderer)
{
	SDL_SetRenderDrawColor(p_renderer, 11, 11, 22, 255);
	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			node* n = &m_nodes[y * MAP_WIDTH + x];
			for(auto&& c : n->m_neighbours)
				SDL_RenderDrawLine(p_renderer, 
								   n->m_position.x * CELL_SIZE + CELL_SIZE / 2,
								   n->m_position.y * CELL_SIZE + CELL_SIZE / 2,
								   c->m_position.x * CELL_SIZE + CELL_SIZE / 2,
								   c->m_position.y * CELL_SIZE + CELL_SIZE / 2);
		}

	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			node* n = &m_nodes[y * MAP_WIDTH + x];
			SDL_Rect rect = { (int) n->m_position.x * CELL_SIZE,
							  (int) n->m_position.y * CELL_SIZE,
							  CELL_SIZE, CELL_SIZE };

			if(n == m_node_start)			SDL_SetRenderDrawColor(p_renderer, 0, 255, 0, 255);
			else if(n == m_node_end)		SDL_SetRenderDrawColor(p_renderer, 255, 0, 0, 255);
			else if(n->m_data & OBSTACLE)	SDL_SetRenderDrawColor(p_renderer, 255, 255, 0, 255);
			else							SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, 255);

			SDL_RenderDrawRect(p_renderer, &rect);
		}

	//if(m_node_end)
	//{
	//	node* current = m_node_end;
	//	//SDL_SetRenderDrawColor(p_renderer, 255, 0, 255, 255);
	//	int i = 0;
	//	while(current->m_parent != nullptr)
	//	{
	//		int blue = (int)(255 * ((i + 1.0f) / 6));
	//		SDL_SetRenderDrawColor(p_renderer, 255, blue, 255, 255);
	//		std::cout << blue << "\n";
	//		SDL_RenderDrawLine(p_renderer,
	//						   current->m_position.x * CELL_SIZE + CELL_SIZE / 2,
	//						   current->m_position.y * CELL_SIZE + CELL_SIZE / 2,
	//						   current->m_parent->m_position.x * CELL_SIZE + CELL_SIZE / 2,
	//						   current->m_parent->m_position.y * CELL_SIZE + CELL_SIZE / 2);
	//		current = current->m_parent;
	//		i++;
	//	}
	//}

	for(size_t i = 0; i < m_path.size() - 1; i++)
	{
		int red = 255 - (int)(255 * ((i + 1.0f) / m_path.size()));
		int blue = (int)(255 * ((i + 1.0f) / m_path.size())); 
		SDL_SetRenderDrawColor(p_renderer, red, blue, 255, 255);
		auto current = &m_path[i];
		auto next = &m_path[i + 1];
		SDL_RenderDrawLine(p_renderer,
						   current->x * CELL_SIZE + CELL_SIZE / 2,
						   current->y * CELL_SIZE + CELL_SIZE / 2,
						   next->x * CELL_SIZE + CELL_SIZE / 2,
						   next->y * CELL_SIZE + CELL_SIZE / 2);
	}
}

void test_state::exit()
{
}

float test_state::heuristic(node* lhs, node* rhs)
{
	return vector2::distance(lhs->m_position, rhs->m_position);
}

void test_state::a_star_path(std::vector<vector2>& out_path)
{
	// Reset the grid and put the values on their "defaults"
	for(int x = 0; x < MAP_WIDTH; x++)
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			m_nodes[y * MAP_WIDTH + x].m_data = m_nodes[y * MAP_WIDTH + x].m_data & OBSTACLE;
			m_nodes[y * MAP_WIDTH + x].f = INFINITY;
			m_nodes[y * MAP_WIDTH + x].g = INFINITY;
			m_nodes[y * MAP_WIDTH + x].m_parent = nullptr;
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
							  { return lhs->f < rhs->f; });
		
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

