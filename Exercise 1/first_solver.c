#include "first_solver.h"
#include "maze.h"
#include "utils/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Gets the euclidean distance between the specified points. This was determined to be the best distance to use after testing, utilizing
//5000 random mazes per distance equation.
static size_t get_distance(Point p1, Point p2) {
    return (size_t)sqrt((int)pow(abs(p1.x - p2.x), 2) + (int)pow(abs(p1.y - p2.y), 2));
}

//Chooses what move to make based on the current knowledge an information of the maze. The move to be made will be stored in "chosenMove", if 
//no moves are possible the function returns 0, otherwise 1.
static int choose_move(const MazeInfo mi, Moves *chosenMove) {
    //Instead of checking each direction manually we check all of them using a loop.
    Point checkDirections[4] = {point_create(-1, 0), point_create(1, 0), point_create(0, -1), point_create(0, 1)};
    Moves moveDirections[4] = {MOVE_L, MOVE_R, MOVE_U, MOVE_D};
    //We set the best distance to an impossibly large size, to ensure any valid distance will be better.
    size_t bestDistance = INT_MAX;
    int setDirection = 0;

    for (size_t i = 0; i < 4; i++) {
        Point newPosition = point_create(mi->robot->position.x + checkDirections[i].x, mi->robot->position.y + checkDirections[i].y);
        if (!get_position(mi, &newPosition) && newPosition.x >= 0 && newPosition.y >= 0) {
            size_t newDistance = get_distance(newPosition, mi->end);
            //If the new distance is better we update it. If the new and current best distance are the same the decision is made randomly.
            if (newDistance < bestDistance || (newDistance == bestDistance && rand_range(1, 10) > 5)) {
                setDirection = 1;

                bestDistance = newDistance;
                *chosenMove = moveDirections[i];
            }
        }
    }

    return setDirection;
}

void no_sensor_solver(MazeInfo mi) {
    //The backtracking stack is used so the robot can go back through the path they followed in case they reach a dead end.
    Stack backtrackingStack = stack_create();

    while (!point_equal(mi->robot->position, mi->end)) {
        Moves chosenMove;
        //If the robot is standing in a position then it has been visited.
        mark_position(mi, &(mi->robot->position));

        //If no valid moves where found we use the stack to go back to the previous position.
        if (!choose_move(mi, &chosenMove)) {
            //Get the previous move from the stack.
            Moves *previousMove;
            previousMove = stack_pop(&backtrackingStack, (DestroyFunction)move_destroy, (CopyFunction)move_copy);

            //If this happens it means the maze has no solution.
            assert(previousMove != NULL);

            Moves inverseMove = invert_move(*previousMove);
            //Moving backwards also counts as moving, print the move.
            print_move(inverseMove);

            //Since we are moving to a previously visited cell we know it's valid, no need to verify.
            point_move(&(mi->robot->position), inverseMove);

            move_destroy(previousMove);
        }
        else {
            //Perform the move to the position given by the move function, then check if it's valid. Note the use of "_Point", since "Point"
            //is a pointer and would require memory allocation.
            Point newPosition = point_create(mi->robot->position.x, mi->robot->position.y);
            point_move(&newPosition, chosenMove);

            //If we moved into a valid position update the robot's position and add the move to the stack.
            if (valid_position(mi, newPosition)) {
                //If the position is valid print it.
                print_move(chosenMove);

                mi->robot->position.y = newPosition.y;
                mi->robot->position.x = newPosition.x;

                stack_push(&backtrackingStack, (CopyFunction)move_copy, &chosenMove);
            }
            //If we moved into a wall mark it as such and don't move the robot.
            else
                mark_position(mi, &(newPosition));
        }
    }

    stack_destroy(backtrackingStack, (DestroyFunction)move_destroy);
}