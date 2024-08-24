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

    mi->start.x = 0;
    mi->start.y = 0;

    mi->end.x = 0;
    mi->end.y = 0;

    mi->robot = malloc(sizeof(_Robot));
    assert(mi->robot != NULL);

    mi->robot->position.x = 0;
    mi->robot->position.y = 0;
    mi->robot->knowledgeGrid = NULL;

    return mi;
}

void mazeinfo_destroy(MazeInfo mi) {

}

void initialize_robot(MazeInfo mi) {
    //The robots initial position is the starting position.
    mi->robot->position.y = mi->start.y;
    mi->robot->position.x = mi->start.x;

    //We create the robots "knowledge gird", used to mark the information the robot has about each cell.
    mi->robot->knowledgeGrid = malloc(mi->maze->sizeY * sizeof(unsigned int*));
    assert(mi->robot->knowledgeGrid != NULL);

    for (size_t y = 0; y < mi->maze->sizeY; y++) {
        mi->robot->knowledgeGrid[y] = malloc(mi->maze->sizeX * sizeof(unsigned int));
        assert(mi->robot->knowledgeGrid[y] != NULL);

        for (size_t x = 0; x < mi->maze->sizeX; x++) {
            mi->robot->knowledgeGrid[y][x] = K_UNKNOWN;
        }
    }
}

int valid_position(const Maze m, const Point p) {
    return m->maze[p.y][p.x] == EMPTY;
}

int point_equal(const Point p1, const Point p2) {
    return p1.y == p2.y && p1.x == p2.x;
}

void point_move(Point *point, Moves move) {
    switch (move) {
    case MOVE_L:
        (point->x)--;
        break;
    case MOVE_R:
        (point->x)++;
        break;
    case MOVE_U:
        (point->y)--;
        break;
    case MOVE_D:
        (point->y)++;
        break;
    }
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

void print_move(const Moves move) {
    switch (move) {
    case MOVE_L:
        fprintf(stdout, "L");
        break;
    case MOVE_R:
        fprintf(stdout, "R");
        break;
    case MOVE_D:
        fprintf(stdout, "D");
        break;
    case MOVE_U:
        fprintf(stdout, "U");
        break;
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