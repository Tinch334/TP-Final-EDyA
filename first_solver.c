#include "maze.h"
#include "stack/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Moves the given point one unit in the given direction, then prints the corresponding move character.
static void point_move(Point point, Moves move) {
    switch (move) {
    case MOVE_L:
        (point->x)--;
        fprintf(stdout, "L");
        break;
    case MOVE_R:
        (point->x)++;
        fprintf(stdout, "R");
        break;
    case MOVE_U:
        (point->y)--;
        fprintf(stdout, "U");
        break;
    case MOVE_D:
        (point->y)++;
        fprintf(stdout, "D");
        break;
    }
}

//Chooses what move to make based on the current knowledge an information of the maze. The move to be made will be stores in "chosenMove", if 
//no moves are possible the function returns 0, otherwise 1.
static int choose_move(const MazeInfo mi, MazeKnowledge discoveredMaze[mi->maze->sizeY][mi->maze->sizeX], Moves *chosenMove) {
    size_t currentYPos = mi->robot->position->y;
    size_t currentXPos = mi->robot->position->x;

    //Check if we are next to the exit, if so move to it. Note that this only checks for direct adjacency, meaning being diagonal to the exit
    //does not count, this is because the robot can't move diagonally.
    int yDiff = (int)currentYPos - mi->end->y;
    int xDiff = (int)currentXPos - mi->end->x;

    if (yDiff == 0 && xDiff == 1) {
        *chosenMove = MOVE_L;
        return 1;
    }
    else if (yDiff == 0 && xDiff == (-1)) {
        *chosenMove = MOVE_R;
        return 1;
    }
    else if (xDiff == 0 && yDiff == 1) {
        *chosenMove = MOVE_U;
        return 1;
    }
    else if (xDiff == 0 && yDiff == (-1)) {
        *chosenMove = MOVE_D;
        return 1;
    }

    //We check all possible move directions, if they are valid they are added to the possible moves, afterwards we generate a random number
    //and choose the corresponding index.
    Moves possibleMoves[4];
    size_t moveIndex = 0;

    //Move left, first we check that we are not on the left edge of the maze.
    //fprintf(stdout, "\nCurrentY: %ld - CurrentX: %ld\n", currentYPos, currentXPos);

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
    size_t chosenIndex = rand_range(0, moveIndex - 1);
    *chosenMove = possibleMoves[chosenIndex];

    return 1;
}

//Takes a move and returns it's inverse.
static Moves invert_move(const Moves move) {
    switch (move) {
    case MOVE_L:
        return MOVE_R;
    case MOVE_R:
        return MOVE_L;
    case MOVE_D:
        return MOVE_U;
    case MOVE_U:
        return MOVE_D;
    }
}

void no_sensor_solver_v1(MazeInfo mi) {
    size_t mazeSizeY = mi->maze->sizeY;
    size_t mazeSizeX = mi->maze->sizeX;
    //This array denotes the parts of the maze the robot has "discovered".
    MazeKnowledge discoveredMaze[mazeSizeY][mazeSizeX];
    //The backtracking stack is used so the robot can go back through the path they followed in case they reach a dead end.
    Stack backtrackingStack = stack_create();

    //Set all the tiles in the discovered maze to unknown.
    for (size_t y = 0; y < mazeSizeY; y++)
        for (size_t x = 0; x < mazeSizeX; x++)
            discoveredMaze[y][x] = K_UNKNOWN;

    while (!point_equal(mi->robot->position, mi->end)) {
        Moves chosenMove;

        //If the robot is standing in a position then it has been visited.
        discoveredMaze[mi->robot->position->y][mi->robot->position->x] = K_VISITED;

        //If no valid moves where found we use the stack to "go back" to the previous position.
        if (!choose_move(mi, discoveredMaze, &chosenMove)) {
            //Get the previous move from the stack.
            Moves *previousMove = malloc(sizeof(Moves));
            previousMove = stack_pop(&backtrackingStack, (DestroyFunction)move_destroy, (CopyFunction)move_copy);
            assert(previousMove != NULL);
            //Move to the inverse of the previous move. Since doing the inverse means we are going to an already visited cell we know it's
            //valid, no need to verify.
            point_move(mi->robot->position, invert_move(*previousMove));

            free(previousMove);
        }
        else {
            //Perform the move to the position given by the move function, then check if it's valid. Note the use of "_Point", since "Point"
            //is a pointer and would require memory allocation.
            _Point newPosition = {.x = mi->robot->position->x, .y = mi->robot->position->y};
            point_move(&newPosition, chosenMove);

            //If we moved into a valid position update the robot's position and add the move to the stack.
            if (valid_position(mi->maze, &newPosition)) {
                mi->robot->position->y = newPosition.y;
                mi->robot->position->x = newPosition.x;

                //IMPORTANT!!!!
                //REMOVE, FOR DISPLAY ONLY.
                //IMPORTANT!!!!
                discoveredMaze[mi->robot->position->y][mi->robot->position->x] = K_VISITED;

                stack_push(&backtrackingStack, (CopyFunction)move_copy, &chosenMove);
            }
            //If we moved into a wall mark it as such and don't move the robot.
            else
                discoveredMaze[newPosition.y][newPosition.x] = K_WALL;
        }
    }
}