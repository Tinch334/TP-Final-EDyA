#ifndef __POINT_H__
#define __POINT_H__

//A generic point structure.
typedef struct {
    int x;
    int y;
} _Point;
typedef _Point *Point;

//Returns 1 if the given points have the same X and Y coordinates.
int point_equal(const Point p1, const Point p2);

#endif /* __POINT_H__ */