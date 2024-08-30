#include "second_solver.h"
#include "maze.h"
#include "utils/stack.h"
#include "utils/custom_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Clamps the given value between min and max.
static clamp(int d, int min, int max) {
  const int t = d < min ? min : d;
  return t > max ? max : t;
}

static int get_heuristic(MazeInfo mi, Point p1, Point p2) {
    if (mi->robot->knowledgeGrid[p1.y][p1.x].knowledge == K_WALL || mi->robot->knowledgeGrid[p2.y][p2.x].knowledge == K_WALL)
        return INT_MAX;

    //if (mi->robot->knowledgeGrid[p1.y][p1.x].knowledge == K_VISITED || mi->robot->knowledgeGrid[p2.y][p2.x].knowledge == K_VISITED)
        //return abs(p1.x - p2.x) + abs(p1.y - p2.y) + 1;

    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
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
    //REMOVE
    fprintf(stderr, "Sensor use pos X: %d - Y:%d\n", mi->robot->position.x, mi->robot->position.y);
    fprintf(stderr, "UP: %d - DOWN: %d - LEFT: %d - RIGHT: %d\n", sensorResult[0], sensorResult[1], sensorResult[2], sensorResult[3]);
    fflush(stderr);
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
            if (mi->robot->knowledgeGrid[yPos][xPos].knowledge != K_VISITED)
                mi->robot->knowledgeGrid[yPos][xPos].knowledge = K_EMPTY;
        }

        Point newPosition = point_create(mi->robot->position.x + checkDirections[i].x * sensorResult[i],
                mi->robot->position.y + checkDirections[i].y * sensorResult[i]);
        //If the last sensor position is in range and inside the maze we mark it as a wall and set it's cost to inf.
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
    for (size_t y = 0; y < mi->robot->mazeSizeY; y++)
        for (size_t x = 0; x < mi->robot->mazeSizeX; x++)
            mi->robot->knowledgeGrid[y][x].cost = get_heuristic(mi, mi->end, point_create(x, y));
}



Moves invert_move(const Moves move) {
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

void *move_copy(void *move) {
    Moves *moveCopy = malloc(sizeof(Moves));
    assert(moveCopy != NULL);

    *moveCopy = *((Moves *) move);
    return moveCopy;
}

void move_destroy(void *move) {
    free(move);
}

//Gets the neighbour with lowest cost and returns the move to reach it.
static Moves get_move(MazeInfo mi) {
    Point checkDirections[4] = {point_create(0, -1), point_create(0, 1), point_create(-1, 0), point_create(1, 0)};
    Moves moveDirections[4] = {MOVE_U, MOVE_D, MOVE_L, MOVE_R};

    size_t minCost = INT_MAX;
    size_t minIndex;

    for (size_t i = 0; i < 4; i++) {
        Point newPosition = point_create(mi->robot->position.x + checkDirections[i].x, mi->robot->position.y + checkDirections[i].y);
        fprintf(stderr, "New pos - X: %d - Y: %d\n", newPosition.x, newPosition.y);

        if (valid_position(mi, newPosition) && mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge != K_WALL) {
            size_t h = mi->robot->knowledgeGrid[newPosition.y][newPosition.x].cost;
            fprintf(stderr, "Valid pos - X: %d - Y: %d - min: %ld - h: %ld\n", newPosition.x, newPosition.y, minCost, h);

            if (minCost > h) {
                fprintf(stderr, "Updated pos - X: %d - Y: %d\n", newPosition.x, newPosition.y);
                minCost = h;
                minIndex = i;
            }
        }
    }

    fprintf(stderr, "Index %d\n", minIndex);

    if (minCost > mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x].cost) {
        fprintf(stderr, "Popped\n");
        mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x].cost = minCost;

        if (!stack_empty(mi->robot->pathStack)) {
            Moves *previousMove = malloc(sizeof(Moves));
            previousMove = stack_pop(&(mi->robot->pathStack), (DestroyFunction)move_destroy, (CopyFunction)move_copy);
            //If this happens it means the maze has no solution.
            assert(previousMove != NULL);
            Moves invertedMove = invert_move(*previousMove);
            move_destroy(previousMove);

            fprintf(stderr, "Inverted move: %d\n", invertedMove);

            return invertedMove;
        }
        return MOVE_N;        
    }
    else {
        return moveDirections[minIndex];
    }
}

static void debug_print_maze_info(MazeInfo mi) {
    fprintf(stderr, "\n------------------------\n");
        fprintf(stderr, "X: %d - Y: %d\n", mi->robot->position.x, mi->robot->position.y);

    for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
        for (size_t x = 0; x < mi->robot->mazeSizeX; x++)
            if (point_equal(mi->robot->position, point_create(x, y)))
                fprintf(stderr, "@");
            else if (mi->robot->knowledgeGrid[y][x].knowledge == K_WALL)
                fprintf(stderr, "#");
            else if (mi->robot->knowledgeGrid[y][x].knowledge == K_EMPTY)
                fprintf(stderr, ".");
            else if (mi->robot->knowledgeGrid[y][x].knowledge == K_VISITED)
                fprintf(stderr, ",");
            else
                fprintf(stderr, "U");

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");

    for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
        for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
            int cost = mi->robot->knowledgeGrid[y][x].cost;
            if (cost == INT_MAX)
                fprintf(stderr, "I ");
            else
                fprintf(stderr, "%d ", mi->robot->knowledgeGrid[y][x].cost);
        }

        fprintf(stderr, "\n");
    }
}

void sensor_solver(MazeInfo mi) {
    CString pathString = cstring_create(10);

    fprintf(stderr, "Dimensions: X: %ld - Y: %ld\n", mi->robot->mazeSizeX, mi->robot->mazeSizeY);

    int cont = 0;

    use_sensor(mi);
    update_costs(mi);

    while(!point_equal(mi->robot->position, mi->end) && cont++ < 35) {
        debug_print_maze_info(mi);

        Moves chosenMove = get_move(mi);

        if (chosenMove == MOVE_N)
            continue;

        Point newPosition = point_create(mi->robot->position.x, mi->robot->position.y);
        char moveChar = point_move(&(newPosition), chosenMove);

        fprintf(stderr, "Chosen pos X: %d - Y: %d\n", newPosition.x, newPosition.y);
        //If the robot is going to move to an unknown location use the sensor.
        if (mi->robot->knowledgeGrid[newPosition.y][newPosition.x].knowledge == K_UNKNOWN) {
            //Walls were detected, recalculate costs.
            fprintf(stderr, "MOved unknown\n");
            if (use_sensor(mi) > 0) {

                //update_costs(mi);
            }
        }
        else {
            stack_push(&(mi->robot->pathStack), (CopyFunction)move_copy, &(chosenMove));
            fprintf(stderr, "Pushed move: %d\n", chosenMove);

            mi->robot->position = point_create(newPosition.x, newPosition.y);
            cstring_add_char(pathString, moveChar);
            mi->robot->knowledgeGrid[mi->robot->position.y][mi->robot->position.x].knowledge = K_VISITED;
            fprintf(stderr, "Pasos: %d\n", cont);
            fflush(stderr);
        }
    }
    printf("! %s\n", pathString->string);
    fflush(stderr);

    cstring_destroy(pathString);
}