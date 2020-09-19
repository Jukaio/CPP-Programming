
#ifndef INCLUDED_SYSTEM_INFO_H
#define INCLUDED_SYSTEM_INFO_H

#include "vector2.h"

#define FRAME_TIME 16

struct SDL_Renderer;
struct grid;

typedef struct window_info
{
    const char* title;
    vector2 position;
    vector2 dimension;
} window_info;


#endif // !INCLUDED_SYSTEM_INFO_H
