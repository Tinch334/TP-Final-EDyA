#include "maze.h"
#include "stack/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/*
    INTERNAL FUNCTIONS
*/
//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Returns one if the given position is empty in the given maze.
static int valid_position(const Maze m, const Point p) {
    return m->maze[p->y][p->x] == EMPTY;
}

//Returns 1 if the given points have the same X and Y coordinates.
static int equal_points(const Point p1, const Point p2) {
    return p1->x == p2->x && p1->y == p2->y;
}

//Moves the given point one unit in the given direction
static void point_move(Point point, Moves move) {
    switch (move) {
    case MOVE_L:
        point->x++;
        puts("Move left");
        break;
    case MOVE_R:
        (point->x)++;
        puts("Move right");
        break;
    case MOVE_U:
        (point->y)--;
        puts("Move up");
        break;
    case MOVE_D:
        (point->y)++;
        puts("Move down");
        break;
    }
}

//Chooses what move to make based on the current knowledge an information of the maze. The move to be made will be stores in "chosenMove", if 
//no moves are possible the function returns 0, otherwise 1.
static int choose_move(const MazeInfo mi, MazeKnowledge discoveredMaze[mi->maze->sizeY][mi->maze->sizeX], Moves *chosenMove) {
    size_t currentYPos = mi->position->y;
    size_t currentXPos = mi->position->x;

    //Check if we are next to the exit, if so move to it. Note that this only checks for direct adjacency, meaning being diagonal to the exit
    //does not count, this is because the robot can't move diagonally.
    int yDiff = currentYPos - mi->end->y;
    int xDiff = currentXPos - mi->end->x;

    if (yDiff == 0) {
        if (xDiff == 1)
            *chosenMove = MOVE_L;
        else
            *chosenMove = MOVE_R;

        return 1;
    }
    else if (xDiff == 0) {
        if (yDiff == 1)
            *chosenMove = MOVE_U;
        else
            *chosenMove = MOVE_D;

        return 1;
    }

    //We check all possible move directions, if they are valid they are added to the possible moves, afterwards we generate a random number
    //and choose the corresponding index.
    Moves possibleMoves[4];
    size_t moveIndex = 0;

    //Move left, first we check that we are not on the left edge of the maze.
    if (currentXPos > 0)
        if (discoveredMaze[currentYPos][currentXPos - 1] == K_UNKNOWN)
            possibleMoves[moveIndex++] = MOVE_L;

    //Move right, first we check that we are not on the right edge of the maze.
    if (currentXPos < mi->maze->sizeX - 1)
        if (discoveredMaze[currentYPos][currentXPos + 1] == K_UNKNOWN)
            possibleMoves[moveIndex++] = MOVE_R;

    //Move up, first we check that we are not on the top edge of the maze.
    if (currentYPos > 0)
        if (discoveredMaze[currentYPos - 1][currentXPos] == K_UNKNOWN)
            possibleMoves[moveIndex++] = MOVE_U;

    //Move up, first we check that we are not on the bottom edge of the maze.
    if (currentYPos < mi->maze->sizeY - 1)
        if (discoveredMaze[currentYPos + 1][currentXPos] == K_UNKNOWN)
            possibleMoves[moveIndex++] = MOVE_D;

    //No valid moves where found.
    if (moveIndex == 0)
        return 0;

    //Choose a random move from all available moves.
    size_t chosenIndex = rand_range(0, moveIndex);
    *chosenMove = possibleMoves[chosenIndex];

    return 1;
}


/*
    PUBLIC FUNCTIONS
*/
MazeInfo mazeinfo_create() {
    //Assign all the necessary memory and check that it was actually assigned.
    MazeInfo mi = malloc(sizeof(_MazeInfo));
    assert(mi != NULL);

    mi->maze = malloc(sizeof(_Maze));
    assert(mi->maze != NULL);
    mi->maze->sizeY = 0;
    mi->maze->sizeX = 0;
    mi->maze->maze = NULL;

    mi->start = malloc(sizeof(_Point));
    assert(mi->start != NULL);
    mi->start->x = 0;
    mi->start->y = 0;

    mi->end = malloc(sizeof(_Point));
    assert(mi->end != NULL);
    mi->end->x = 0;
    mi->end->y = 0;

    mi->position = malloc(sizeof(_Point));
    assert(mi->position != NULL);
    mi->position->x = 0;
    mi->position->y = 0;

    mi->sensorRange = 0;

    return mi;
}

void mazeinfo_destroy(MazeInfo mi) {
    //Since "maze" is a double pointer we first free all the memory pointed to by the second pointer.
    for (size_t y = 0; y < mi->maze->sizeY; y++)
        free(mi->maze->maze[y]);
    free(mi->maze->maze);
    free(mi->maze);

    free(mi->start);
    free(mi->end);
    free(mi->position);

    free(mi);
}

char *no_sensor_solver_v1(MazeInfo mi) {
    size_t mazeSizeY = mi->maze->sizeY;
    size_t mazeSizeX = mi->maze->sizeX;
    //This array denotes the parts of the maze the robot has "discovered".
    MazeKnowledge discoveredMaze[mazeSizeY][mazeSizeX];

    //Set all the tiles in the discovered maze to unknown.
    for (size_t y = 0; y < mazeSizeY; y++)
        for (size_t x = 0; x < mazeSizeX; x++)
            discoveredMaze[y][x] = K_UNKNOWN;

    while (!equal_points(mi->position, mi->end)) {
        //If the robot is standing in a position then it has been visited.
        discoveredMaze[mi->start->y][mi->start->x] = K_VISITED;
        Moves chosenMove;

        //If no valid moves where found we use the stack to "go back" to the previous position.
        if (!choose_move(mi, discoveredMaze, &chosenMove)) {
            //IMPORTANT!!!!
            //Implement backtracking stack. Remember to add the performed move to a stack if it's valid.
        }
        //Perform the chosen move.
        else {
            //Perform the move to the position given by the move function, then check if it's valid. Note the use of "_Point", since "Point"
            //is a pointer and would require memory allocation.
            _Point newPosition = {.x = mi->position->x, .y = mi->position->y};
            point_move(&newPosition, chosenMove);

            //If we moved into a valid position update the robots position.
            if (valid_position(mi->maze, &newPosition)) {
                mi->position->y = newPosition.y;
                mi->position->x = newPosition.x;
            }

            //If we moved into a wall mark it as such and don't move the robot.
            else
                discoveredMaze[newPosition.y][newPosition.x] = K_WALL;
        }
    }

    return NULL;
}