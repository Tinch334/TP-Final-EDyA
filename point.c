#include "point.h"
#include <stdlib.h>
#include <assert.h>

//Returns 1 if the given points have the same X and Y coordinates.
int point_equal(const Point p1, const Point p2) {
    return p1->x == p2->x && p1->y == p2->y;
}