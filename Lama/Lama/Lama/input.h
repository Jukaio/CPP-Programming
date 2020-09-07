
#ifndef INCLDUED_MOUSE_H
#define INCLUDED_MOUSE_H

#include "vector2.h"

typedef unsigned long Uint32;

enum mouse_buttons
{
    NONE = 0,
    LEFT_CLICK = 1 << 0,
    MIDDLE_CLICK = 1 << 1,
    RIGHT_CLICK = 1 << 2,
};

typedef struct mouse
{
    vector2 position;
    Uint32 buttons;
} mouse;

#endif // !INCLDUED_MOUSE_H
