#ifndef __MAZE_H__
#define __MAZE_H__
#include "point.h"
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

//The maze structure, we use a double pointer to be able to define a 2D array of unspecified dimensions, this method requires more setup than
//using a linear array as 2D, but simplifies accessing the array.
typedef struct {
    size_t sizeY;
    size_t sizeX;
    MazeStates **maze;
} _Maze;
typedef _Maze *Maze;

//Contains all necessary information to solve the maze.
typedef struct {
    Maze maze;
    Point start;
    Point end;
    Point position;
    size_t sensorRange;
} _MazeInfo;
typedef _MazeInfo *MazeInfo;


//Creates a maze structure, all the values are set to zero/NULL.
MazeInfo mazeinfo_create();

//Destroys the given maze structure.
void mazeinfo_destroy(MazeInfo mi);

//Takes a "MazeInfo" structure, returns a path from the start to the end position, the algorithm assumes that there's at least one valid path.
//The string returned by the function needs to be freed after use.
char *no_sensor_solver_v1(MazeInfo mi);

#endif /* __MAZE_H_ */