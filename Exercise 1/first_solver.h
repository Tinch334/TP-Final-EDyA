#ifndef __FIRST_SOLVER_H__
#define __FIRST_SOLVER_H__
#include "maze.h"
#include <stdlib.h>

//Takes a "MazeInfo" structure, prints the movements to go from the start to the end position. The algorithm follows the given guidelines
//and assumes that there's at least one valid path.
void no_sensor_solver(MazeInfo mi);

#endif /* __FIRST_SOLVER_H__ */