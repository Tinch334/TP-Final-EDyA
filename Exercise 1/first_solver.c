#include "first_solver.h"
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

//Chooses what move to make based on the current knowledge an information of the maze. The move to be made will be stored in "chosenMove", if 
//no moves are possible the function returns 0, otherwise 1. When choosing the function prioritizes going to the direction of the exit, whenever
//possible.
static int choose_move(const MazeInfo mi, Moves *chosenMove) {
    size_t currentYPos = mi->robot->position.y;
    size_t currentXPos = mi->robot->position.x;

    //Check if we are next to the exit, if so move to it. Note that this only checks for direct adjacency, meaning being diagonal to the exit
    //does not count, this is because the robot can't move diagonally.
    int yDiff = (int)currentYPos - mi->end.y;
    int xDiff = (int)currentXPos - mi->end.x;

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

    //A priority queue could be used here to avoid having to arrays, however for only four elements and two priority levels the added
    //complexity would not be worth it.t 
    //The priority array is used to store move directions that would lead the robot to the direction of the exit.
    Moves priorityMoves[2];
    size_t priorityIndex = 0;
    //We check all possible move directions, if they are valid they are added to the possible moves, afterwards we generate a random number
    //and choose the corresponding index.
    Moves possibleMoves[3];
    size_t moveIndex = 0;

    //Move left, first we check that we are not on the left edge of the maze.
    if (currentXPos > 0)
        if (mi->robot->knowledgeGrid[currentYPos][currentXPos - 1] == K_UNKNOWN)
            if (xDiff > 0)
                priorityMoves[priorityIndex++] = MOVE_L;
            else
                possibleMoves[moveIndex++] = MOVE_L;

    //Move right, first we check that we are not on the right edge of the maze.
    if (currentXPos < mi->maze->sizeX - 1)
        if (mi->robot->knowledgeGrid[currentYPos][currentXPos + 1] == K_UNKNOWN)
            if (xDiff < 0)
                priorityMoves[priorityIndex++] = MOVE_R;
            else
                possibleMoves[moveIndex++] = MOVE_R;

    //Move up, first we check that we are not on the top edge of the maze.
    if (currentYPos > 0)
        if (mi->robot->knowledgeGrid[currentYPos - 1][currentXPos] == K_UNKNOWN)
            if (yDiff > 0)
                priorityMoves[priorityIndex++] = MOVE_U;
            else
                possibleMoves[moveIndex++] = MOVE_U;

    //Move up, first we check that we are not on the bottom edge of the maze.
    if (currentYPos < mi->maze->sizeY - 1)
        if (mi->robot->knowledgeGrid[currentYPos + 1][currentXPos] == K_UNKNOWN)
            if (yDiff < 0)
                priorityMoves[priorityIndex++] = MOVE_D;
            else
                possibleMoves[moveIndex++] = MOVE_D;

    //No valid moves where found.
    if (priorityIndex == 0 && moveIndex == 0)
        return 0;

    //If there are any moves in the priority array we select from those.
    if (priorityIndex == 0) {
        //Choose a random move from all available moves.
        size_t chosenIndex = rand_range(0, moveIndex - 1);
        *chosenMove = possibleMoves[chosenIndex];
    }
    else {
        size_t chosenIndex = rand_range(0, priorityIndex - 1);
        *chosenMove = priorityMoves[chosenIndex];
    }

    return 1;
}

void no_sensor_solver(MazeInfo mi) {
    initialize_robot(mi);

    size_t mazeSizeY = mi->maze->sizeY;
    size_t mazeSizeX = mi->maze->sizeX;

    //The backtracking stack is used so the robot can go back through the path they followed in case they reach a dead end.
    Stack backtrackingStack = stack_create();

    while (!point_equal(mi->robot->position, mi->end)) {
        Moves chosenMove;

        //If the robot is standing in a position then it has been visited.
        mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x] = K_VISITED;

        //If no valid moves where found we use the stack to go back to the previous position.
        if (!choose_move(mi, &chosenMove)) {
            //puts("Backtrack");
            //Get the previous move from the stack.
            Moves *previousMove = malloc(sizeof(Moves));
            previousMove = stack_pop(&backtrackingStack, (DestroyFunction)move_destroy, (CopyFunction)move_copy);

            //If this happens it means the maze has no solution.
            assert(previousMove != NULL);

            Moves inverseMove = invert_move(*previousMove);
            //Moving backwards also counts as moving, print the move.
            print_move(inverseMove);

            //Since we are moving to a previously visited cell we know it's valid, no need to verify.
            point_move(&(mi->robot->position), inverseMove);

            free(previousMove);
        }
        else {
            //Perform the move to the position given by the move function, then check if it's valid. Note the use of "_Point", since "Point"
            //is a pointer and would require memory allocation.
            Point newPosition = {.x = mi->robot->position.x, .y = mi->robot->position.y};
            point_move(&newPosition, chosenMove);

            //If we moved into a valid position update the robot's position and add the move to the stack.
            if (valid_position(mi->maze, newPosition)) {
                //If the position is valid print it.
                print_move(chosenMove);

                mi->robot->position.y = newPosition.y;
                mi->robot->position.x = newPosition.x;

                stack_push(&backtrackingStack, (CopyFunction)move_copy, &chosenMove);
            }
            //If we moved into a wall mark it as such and don't move the robot.
            else
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x] = K_WALL;
        }
    }
}