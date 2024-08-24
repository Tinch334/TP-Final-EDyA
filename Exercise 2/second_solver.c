#include "second_solver.h"
#include "maze.h"
#include "priority_queue/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>


//Generates a random number between min and max. Note that the output is not perfectly distributed, however a more balanced version would
//require significantly more code for a marginal improvement.
static int rand_range(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

//Moves the given point one unit in the given direction.
static void point_move(Point point, Moves move) {
    switch (move) {
    case MOVE_L:
        (point.x)--;
        break;
    case MOVE_R:
        (point.x)++;
        break;
    case MOVE_U:
        (point.y)--;
        break;
    case MOVE_D:
        (point.y)++;
        break;
    }
}

static int get_heuristic(Point p1, Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

//Initialize the values of all the cells in the maze.
static void initialize_robot(Robot r) {
    r->maze->maze = malloc(r->maze->sizeY * sizeof(MazeCell*));
    assert(r->maze->maze != NULL);

    for (size_t y = 0; y < r->maze->sizeY; y++) {
        r->maze->maze[y] = malloc(r->maze->sizeX * sizeof(MazeCell));
        assert(r->maze->maze[y] != NULL);

        for (size_t x = 0; x < r->maze->sizeX; x++) {
            r->maze->maze[y][x].g = INT_MAX;
            r->maze->maze[y][x].rhs = INT_MAX;
            r->maze->maze[y][x].knowledge = K_UNKNOWN;
        }
    }

    r->maze->maze[r->start.y][r->start.x].knowledge = K_EMPTY;
    r->maze->maze[r->start.y][r->start.x].rhs = 0;
    pq_insert(r->pq, ppq_create(r->start, get_k1(r, r->start), get_k2(r, r->start)));
}


char *sensor_solver(Robot r) {
    initialize_robot(r);

    for (size_t y = 0; y < r->maze->sizeY; y++) {
        for (size_t x = 0; x < r->maze->sizeX; x++) {
            fprintf(stderr, "%c", r->maze->maze[y][x].rhs == INT_MAX ? 'I' : 'N');
        }
        fprintf(stderr, "\n");
    }

    return NULL;
}