#include "second_solver.h"
#include "maze.h"
#include "utils/queue.h"
#include "utils/custom_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Returns 1 if the given point is inside the maze and not a known wall.
static int valid_position(MazeInfo mi, Point p) {
    return p.x >= 0 && p.x < mi->robot->mazeSizeX && p.y >= 0 && p.y < mi->robot->mazeSizeY;
}

//Uses the sensor in the robot's current position, returns how many walls were detected.
static int use_sensor(MazeInfo mi) {
    int wall_count = 0;
    int sensorResult[4];
    //The checking order matches the order in which the sensor returns directions.
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};

    //Call sensor and store results in the array.
    printf("? %d %d\n", mi->robot->position.y, mi->robot->position.x);
    fflush(stdout);
    scanf("%d%d%d%d", &sensorResult[0], &sensorResult[1], &sensorResult[2], &sensorResult[3]);

    //Estimate the sensors range, getting the largest reading minus 1.
    for (size_t i = 0; i < 4; i++)
        if (sensorResult[i] - 1 > mi->robot->knownSensorRange)
            mi->robot->knownSensorRange = sensorResult[i] - 1;

    //Use new sensor range to detect if there are walls.
    for (size_t i = 0; i < 4; i++) {
        //First mark all known empty spaces.
        for (size_t cnt = 1; cnt < sensorResult[i]; cnt++) {
            int yPos = mi->robot->position.y + checkDirections[i].y * cnt;
            int xPos = mi->robot->position.x + checkDirections[i].x * cnt;
            mi->robot->knowledgeGrid[yPos][xPos].knowledge = K_EMPTY;
        }

        Point newPosition = point_create(mi->robot->position.x + checkDirections[i].x * sensorResult[i],
                mi->robot->position.y + checkDirections[i].y * sensorResult[i]);
        //If the last sensor position is in range and inside the maze we mark it as a wall and set it's cost to infinity.
        if (valid_position(mi, newPosition))
            if (sensorResult[i] <= mi->robot->knownSensorRange) {
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge = K_WALL;
                mi->robot->knowledgeGrid[newPosition.y][newPosition.x].cost = INT_MAX;
                wall_count++;
            }
    }

    return wall_count;
}

//This two functions are for use with the queue, they create and destroy a "*Point" structure.
static Point *p_point_create(Point *p) {
    Point *newPoint = malloc(sizeof(Point));
    assert(newPoint != NULL);

    newPoint->x = p->x;
    newPoint->y = p->y;

    return newPoint;
}

static void p_point_destroy(Point *p) {
    free(p);
}

//Whenever a wall is detected we update all the costs with the new information.
static void update_costs(MazeInfo mi) {
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};
    int done = 0;

    //We set all costs to infinity to recalculate, this also allows us to not mark visited cells for BFS, we can use the cost as the marker.
    for (size_t y = 0; y < mi->robot->mazeSizeY; y++)
        for (size_t x = 0; x < mi->robot->mazeSizeX; x++)
            mi->robot->knowledgeGrid[y][x].cost = INT_MAX;

    //The end always has a cost of zero.
    mi->robot->knowledgeGrid[mi->end.y][mi->end.x].cost = 0;

    Queue q = queue_create();
    queue_enqueue(&q, (CopyFunction)p_point_create, &(mi->end));

    //Recalculate costs in the maze utilizing BFS.
    while (!queue_empty(q) && !done) {
        Point *p = queue_dequeue(&q ,(DestroyFunction)p_point_destroy, (CopyFunction)p_point_create);

        for (size_t i = 0; i < 4; i++) {
            Point newPosition = point_create(p->x + checkDirections[i].x, p->y + checkDirections[i].y);

            //If we've reached the robot we stop, we already have a path.
            if (!point_equal(newPosition, mi->robot->position)) {
                //We check if the new position is valid and not a wall.
                if (valid_position(mi, newPosition) && mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge != K_WALL) {
                    //The cost of moving to a new cell is always 1.
                    size_t newCost =  mi->robot->knowledgeGrid[p->y][p->x].cost + 1;
                    size_t currentCost = mi->robot->knowledgeGrid[newPosition.y][newPosition.x].cost;

                    //If the new cost is lower than the current cost expand the cell. Finally if the new and current cost are equal we
                    //decide randomly.
                    if (newCost < currentCost || (newCost == currentCost && rand_range(1, 10) > 5)) {
                        mi->robot->knowledgeGrid[newPosition.y][newPosition.x].cost = newCost;
                        queue_enqueue(&q, (CopyFunction)p_point_create, &newPosition);
                    }
                }
            }
            else
                done = 1;
        }
        p_point_destroy(p);
    }

    queue_destroy(q, (DestroyFunction)p_point_destroy);
}

//Gets the neighbour with lowest cost and returns the move to reach it.
static Moves get_move(MazeInfo mi) {
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};
    Moves moveDirections[4] = {MOVE_U, MOVE_D, MOVE_L, MOVE_R};

    size_t minCost = INT_MAX;
    size_t minCostIndex;

    //Get neighbour with lowest cost.
    for (size_t i = 0; i < 4; i++) {
        Point newPosition = point_create(mi->robot->position.x + checkDirections[i].x, mi->robot->position.y + checkDirections[i].y);
    
        //Check that neighbour is valid.
        if (valid_position(mi, newPosition)){
            int neighbourCost = mi->robot->knowledgeGrid[newPosition.y][newPosition.x].cost;

            //If both costs are equal choose randomly.
            if (neighbourCost < minCost || (neighbourCost == minCost && rand_range(1, 10) > 5)) {
                minCost = neighbourCost;
                minCostIndex = i;
            }
        }
    }

    return moveDirections[minCostIndex];
}

void sensor_solver(MazeInfo mi) {
    CString pathString = cstring_create(10);

    //Initial scan and cost calculation of the start area.
    use_sensor(mi);
    update_costs(mi);

    while(!point_equal(mi->robot->position, mi->end)) {
        for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
            for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
                if (mi->robot->position.x == x && mi->robot->position.y == y)
                    fprintf(stderr, "@");
                else if (mi->robot->knowledgeGrid[y][x].knowledge == K_UNKNOWN)
                    fprintf(stderr, "U");
                else if (mi->robot->knowledgeGrid[y][x].knowledge == K_WALL)
                    fprintf(stderr, "#");
                else
                    fprintf(stderr, ".");
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");

        for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
            for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
                if (mi->robot->knowledgeGrid[y][x].cost == INT_MAX)
                    fprintf(stderr, "I");
                else
                    fprintf(stderr, "%d", mi->robot->knowledgeGrid[y][x].cost);
            }
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "\n");
        for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
            for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
                fprintf(stderr, "%d", mi->robot->knowledgeGrid[y][x].knowledge);
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n--------------------------------------\n");

        //debug_print_maze_info(mi);
        Moves chosenMove = get_move(mi);
        Point newPosition = point_create(mi->robot->position.x, mi->robot->position.y);
        char moveChar = point_move(&(newPosition), chosenMove);

        //If the robot is going to move to an unknown location use the sensor.
        if (mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_UNKNOWN) {
            //Walls were detected, recalculate costs.
            if (use_sensor(mi) > 0)
                update_costs(mi);
        }
        else {
            //If the move was valid perform it and mark the corresponding cell.
            mi->robot->position = point_create(newPosition.x, newPosition.y);
            cstring_add_char(pathString, moveChar);
            mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x].knowledge = K_EMPTY;
        }
    }

    //Output travelled path.
    printf("! %s\n", pathString->string);
    fflush(stderr);

    cstring_destroy(pathString);
}