#include "maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


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

    mi->robot = malloc(sizeof(_Robot));
    assert(mi->robot != NULL);
    mi->robot->position = malloc(sizeof(_Point));
    assert(mi->robot->position != NULL);

    mi->robot->position->x = 0;
    mi->robot->position->y = 0;
    mi->robot->sensorRange = 0;

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

    free(mi->robot->position);
    free(mi->robot);

    free(mi);
}

int valid_position(const Maze m, const Point p) {
    return m->maze[p->y][p->x] == EMPTY;
}

int point_equal(const Point p1, const Point p2) {
    return p1->y == p2->y && p1->x == p2->x;
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