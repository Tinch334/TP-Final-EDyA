#ifndef __MAZE_H__
#define __MAZE_H__
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
    K_UNKNOWN
} CellKnowledge;

//To use the algorithm we need to store additional information about the cells.
typedef struct {
    int cost;
    CellKnowledge knowledge;
} MazeCell;

//A structure representing the robot and the knowledge it has. We use a double pointer to be able to define a 2D array of unspecified dimensions
//this method requires more setup than using a linear array as 2D, but simplifies accessing the array.
typedef struct {
    size_t mazeSizeX;
    size_t mazeSizeY;
    MazeCell **knowledgeGrid;
    Point position;
    size_t knownSensorRange;
} _Robot;
typedef _Robot *Robot;

//Contains all necessary information to solve the maze.
typedef struct {
    Robot robot;
    Point start;
    Point end;
} _MazeInfo;
typedef _MazeInfo *MazeInfo;

//Creates a MazeInfo structure, with the values set appropriately.
MazeInfo mazeinfo_create(Point start, Point end, size_t sizeX, size_t sizeY);

//Destroys the given MazeInfo structure.
void mazeinfo_destroy(MazeInfo mi);

//Initializes the robot structure.
void initialize_robot(MazeInfo mi, size_t sizeX, size_t sizeY);

//Creates and returns a point with the given coordinates.
Point point_create(int x, int y);

//Returns 1 if the given points have the same X and Y coordinates.
int point_equal(const Point p1, const Point p2);

//Moves the given point one unit in the given direction and returns the direction as a character.
char point_move(Point *point, Moves move);

#endif /* __MAZE_H_ */