#include "second_solver.h"
#include "utils/custom_string.h"
#include "maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Moves the given point one unit in the given direction and returns the direction as a character.
static char point_move(Point *point, Moves move) {
    switch (move) {
    case MOVE_L:
        (point->x)--;
        return 'L';
    case MOVE_R:
        (point->x)++;
        return 'R';
    case MOVE_U:
        (point->y)--;
        return 'U';
    case MOVE_D:
        (point->y)++;
        return 'D';
    }
}

//Returns 1 if the given point is inside the maze.
static int in_bounds(MazeInfo mi, Point p) {
    return p.x >= 0 && p.x < mi->robot->mazeSizeX && p.y >= 0 && p.y < mi->robot->mazeSizeY;
}

static int get_heuristic(Point p1, Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

static int has_path(MazeInfo mi, Point p) {
    int hasPath = 0;
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};

    for (size_t i = 0; i < 4; i++) {
        Point newPosition = point_create(p.x + checkDirections[i].x, p.y + checkDirections[i].y);

        if (in_bounds(mi, newPosition))
            if (mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_EMPTY || 
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_UNKNOWN)
                hasPath++;
    }

    return hasPath;
}

static Moves update_cell_get_move(MazeInfo mi, int sensorResult[4]) {
    Point robotPos = mi->robot->position;
    //Instead of checking each direction manually we check all of them using a loop.
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};
    Moves moveDirections[4] = {MOVE_U, MOVE_D, MOVE_L, MOVE_R};

    size_t lowestCost = inf(mi);
    int lowestCostIndex = -1;
    
    for (size_t i = 0; i < 4; i++) {
        Point newPosition = point_create(robotPos.x + checkDirections[i].x, robotPos.y + checkDirections[i].y);

        //Ensure we are in bounds.
        if (in_bounds(mi, newPosition)) {
            fprintf(stderr, "New pos - X: %d - Y: %d\n", newPosition.x, newPosition.y);

            //Update the g cost of the cell, in our case moving to a cell always costs the same.
            mi->robot->knowledgeGrid[newPosition.y][newPosition.x].g = mi->robot->knowledgeGrid[robotPos.y][robotPos.x].g + 1;

            //Check if there's a wall in that direction, if so moving to it never gets us to the exit.
            if (sensorResult[i] == 1) {
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h = inf(mi);
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge = K_WALL;
            }
            else {
                /*
                //Check if we haven't already visited the cell.
                if (!(mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_VISITED)) {
                    mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge = K_EMPTY;
                    mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h = get_heuristic(newPosition, mi->end);
                }
                else
                    //If we already moved to the tile then we want to avoid doing it again if possible.
                    mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h = get_heuristic(newPosition, mi->end) + 1;
                */
                //Check if we haven't already visited the cell.
                if (!(mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_VISITED))
                    mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge = K_EMPTY;
            }

            if (has_path(mi, newPosition)) {
                fprintf(stderr, "Has path\n");
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h = get_heuristic(newPosition, mi->end);
            }
            else
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h = inf(mi);

            size_t newCost = mi->robot->knowledgeGrid[newPosition.y][newPosition.x].h + mi->robot->knowledgeGrid[newPosition.y][newPosition.x].g;

            fprintf(stderr, "New cost: %d\n", newCost);

            //If the new cost is better we update it. If the new and current best cost are the same the decision is made randomly.
            if (newCost < lowestCost || (newCost == lowestCost && rand_range(1, 10) > 5)) {
                lowestCost = newCost;
                lowestCostIndex = i;
            }
        }
    }

    if (mi->robot->knowledgeGrid[robotPos.y][robotPos.x].h < lowestCost)
        mi->robot->knowledgeGrid[robotPos.y][robotPos.x].h = lowestCost;

    fprintf(stderr, "LIndex: %d\n", lowestCostIndex);

    return moveDirections[lowestCostIndex];
}

void sensor_solver(MazeInfo mi) {
    //CString pathString = cstring_create(5);
    int sensorResult[4];

    fprintf(stderr, "Dimensions: X: %d - Y: %d\n", mi->robot->mazeSizeX, mi->robot->mazeSizeY);

    while(!point_equal(mi->robot->position, mi->end)) {
        fprintf(stderr, "\n------------------------\n");
        //Use the sensor to scan the robot's surroundings.
        printf("? %d %d\n", mi->robot->position.y, mi->robot->position.x);
        fflush(stdout);
        scanf("%d%d%d%d", &sensorResult[0], &sensorResult[1], &sensorResult[2], &sensorResult[3]);
        fprintf(stderr, "UP: %d - DOWN: %d - LEFT: %d - RIGHT: %d\n", sensorResult[0], sensorResult[1], sensorResult[2], sensorResult[3]);
        fprintf(stderr, "X: %d - Y: %d\n", mi->robot->position.x, mi->robot->position.y);

        for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
            for (size_t x = 0; x < mi->robot->mazeSizeX; x++)
                if (point_equal(mi->robot->position, point_create(x, y)))
                    fprintf(stderr, "@");
                else if (mi->robot->knowledgeGrid[y][x].knowledge == K_WALL)
                    fprintf(stderr, "#");
                else if (mi->robot->knowledgeGrid[y][x].knowledge == K_EMPTY || mi->robot->knowledgeGrid[y][x].knowledge == K_VISITED)
                    fprintf(stderr, ".");
                else
                    fprintf(stderr, "U");

            fprintf(stderr, "\n");
        }

        Moves chosenMove = update_cell_get_move(mi, sensorResult);
        char moveChar = point_move(&(mi->robot->position), chosenMove);
        //cstring_add_char(pathString, moveChar);
        mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x].knowledge = K_VISITED;
        fflush(stderr);
    }

    printf("! L\n");

    //cstring_destroy(pathString);
}