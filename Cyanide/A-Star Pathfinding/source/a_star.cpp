// a_star.h

#include "a_star.h"

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <typeinfo>

#include "maze.h"

namespace CONFIG
{
    const char TITLE[] = "A* - Pathfinding";
    const int WINDOW_WIDTH = 488;
    const int WINDOW_HEIGHT = 488;
    const float WINDOW_SCALE = 1.0f; 
}

// Application Factory - Could get optimised, could be better ¯\_(ツ)_/¯
application* application::implement()
{
    std::cout << "Window width: ";
    int width;
    std::cin >> width;

    std::cout << "Window height: ";
    int height;
    std::cin >> height;

    int scale = 1;
    return new a_star(width, height, CONFIG::WINDOW_SCALE, CONFIG::TITLE);
}

a_star::a_star(int p_window_w, int p_window_h, float p_scale, const char* p_title)
    : application(p_window_w, p_window_h, p_scale, p_title)
{

}

void a_star::application_enter()
{
    SDL_SetWindowResizable(m_window, SDL_TRUE);

    add_state(new maze_state());
    set_state(0);
}

void a_star::application_exit()
{

}
