#ifndef __MAZE_H__
#define __MAZE_H__
#include <stdlib.h>

//Has all the possible states for a cell in the maze.
typedef enum {
    EMPTY,
    WALL
} MazeStates;

//All the possible "knowledge" states for a cell in the knowledge matrix.
typedef enum {
    K_EMPTY,
    K_WALL,
    K_VISITED,
    K_UNKNOWN
} MazeKnowledge;

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

//The maze structure, we use a double pointer to be able to define a 2D array of unspecified dimensions, this method requires more setup than
//using a linear array as 2D, but simplifies accessing the array.
typedef struct {
    size_t sizeY;
    size_t sizeX;
    MazeStates **maze;
} _Maze;
typedef _Maze *Maze;

//A simple structure representing the robot, has it's position and sensor range.
typedef struct {
    Point position;
    MazeKnowledge **knowledgeGrid;
} _Robot;
typedef _Robot *Robot;

//Contains all necessary information to solve the maze.
typedef struct {
    Maze maze;
    Point start;
    Point end;
    Robot robot;
} _MazeInfo;
typedef _MazeInfo *MazeInfo;

//Creates a maze structure, all the values are set to zero/NULL.
MazeInfo mazeinfo_create();

//Destroys the given maze structure.
void mazeinfo_destroy(MazeInfo mi);

//Initializes the robot structure.
void initialize_robot(MazeInfo mi);

//Returns 1 if the given position is empty in the given maze.
int valid_position(const Maze m, const Point p);

//Creates and returns a point with the given coordinates.
Point create_point(int x, int y);

//Returns 1 if the given points have the same X and Y coordinates.
int point_equal(const Point p1, const Point p2);

//Moves the given point one unit in the given direction.
void point_move(Point *point, Moves move);

//Returns the inverse of the given move.
Moves invert_move(const Moves move);

//Prints the given move to the standard output.
void print_move(const Moves move);

//Returns a copy of the given move.
void *move_copy(void *move);

//Destroys the given move.
void move_destroy(void *move);

#endif /* __MAZE_H_ */