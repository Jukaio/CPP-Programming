// vector2.c

#include "vector2.h"

vector2 lerp(vector2 from, vector2 to, int step_ms)
{
    vector2 to_return;
    to_return.x = (to.x - from.x) * (float)(step_ms * 0.001) + from.x;
    to_return.y = (to.y - from.y) * (float)(step_ms * 0.001) + from.y;
    return to_return;
}

bool interpolate(int min, int max, int* current)
{
    if(*current < min)
    {
        *current = min;
        return true;
    }
    else if(*current > max)
    {
        *current = max;
        return true;
    }
    return false;
}