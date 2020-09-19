
#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

#include <stdbool.h>
#include <math.h>

#define NORTH    0
#define EAST     1
#define SOUTH    2
#define WEST     3
#define NUM_DIRS 4

typedef struct vector2
{
    int x;
    int y;
} vector2;

vector2 lerp(vector2 from, 
                    vector2 to, 
                    int step);
bool interpolate(int min, int max, int* current);

#endif // !INCLUDED_VECTOR2_H
