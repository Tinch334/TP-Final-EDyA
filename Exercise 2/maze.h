#ifndef __MAZE_H__
#define __MAZE_H__
#include "priority_queue/bheap.h"
#include <stdlib.h>

typedef enum {
    MOVE_L,
    MOVE_R,
    MOVE_D,
    MOVE_U
} Moves;

//A generic 2D point structure.
typedef struct {
    int x;
    int y;
} Point;

//All the possible "knowledge" states for a cell in the knowledge matrix.
typedef enum {
    K_EMPTY,
    K_WALL,
    K_VISITED,
    K_UNKNOWN
} MazeKnowledge;

//To use the algorithm we need to store additional information about the cells.
typedef struct {
    int g;
    int rhs;
    MazeKnowledge knowledge;
} MazeCell;

//The maze structure, we use a double pointer to be able to define a 2D array of unspecified dimensions, this method requires more setup than
//using a linear array as 2D, but simplifies accessing the array.
typedef struct {
    size_t sizeY;
    size_t sizeX;
    MazeCell **maze;
} _Maze;
typedef _Maze *Maze;

//A structure representing the robot and the knowledge it has.
typedef struct {
    BHeap bh;
    Maze maze;
    Point start;
    Point end;
    Point position;
    size_t sensorRange;
} _Robot;
typedef _Robot *Robot;

//The structure used to store cells in the priority queue.
typedef struct {
    Point position;
    int k1;
    int k2;
} _PositionBH;
typedef _PositionBH *PositionBH;

//Creates a "PositionBH" structure with the given values.
PositionBH pbh_create(Point p, int k1, int k2);

//Destroys the given "PositionBH" structure.
void pbh_destroy(PositionBH pbh);

//Compares two "PositionBH" structures, returns an integer according to the result.
int pbh_compare(PositionBH pbh_1, PositionBH pbh_2);

//Creates a robot structure, with the values set appropriately.
Robot robot_create();

//Destroys the given robot structure.
void robot_destroy(Robot r);

//Returns 1 if the given points have the same X and Y coordinates.
int point_equal(const Point p1, const Point p2);

#endif /* __MAZE_H_ */