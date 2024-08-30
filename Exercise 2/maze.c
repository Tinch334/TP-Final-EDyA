#include "maze.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>


MazeInfo mazeinfo_create(Point start, Point end, size_t sizeX, size_t sizeY) {
    MazeInfo mi = malloc(sizeof(_MazeInfo));
    assert(mi != NULL);
    mi->start.x = start.x;
    mi->start.y = start.y;
    mi->end.x = end.x;
    mi->end.y = end.y;

    mi->robot = malloc(sizeof(_Robot));
    assert(mi->robot != NULL);

    initialize_robot(mi, sizeX, sizeY);

    return mi;
}

void mazeinfo_destroy(MazeInfo mi) {
    for (size_t y = 0; y < mi->robot->mazeSizeY; y++)
        free(mi->robot->knowledgeGrid[y]);
    free(mi->robot->knowledgeGrid);
    free(mi->robot);

    free(mi);
}

void initialize_robot(MazeInfo mi, size_t sizeX, size_t sizeY) {
    //The robots initial position is the starting position.
    mi->robot->position.y = mi->start.y;
    mi->robot->position.x = mi->start.x;

    mi->robot->mazeSizeX = sizeX;
    mi->robot->mazeSizeY = sizeY;

    //The sensor always has a range of at least 1.
    mi->robot->knownSensorRange = 1;

    //We create the robots "knowledge gird", used to mark the information the robot has about each cell.
    mi->robot->knowledgeGrid = malloc(mi->robot->mazeSizeY * sizeof(MazeCell*));
    assert(mi->robot->knowledgeGrid != NULL);

    for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
        mi->robot->knowledgeGrid[y] = malloc(mi->robot->mazeSizeX * sizeof(MazeCell));
        assert(mi->robot->knowledgeGrid[y] != NULL);

        for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
            mi->robot->knowledgeGrid[y][x].knowledge = K_UNKNOWN;
            mi->robot->knowledgeGrid[y][x].cost = INT_MAX;
        }
    }

    //We know the end and start are empty.
    mi->robot->knowledgeGrid[mi->end.y][mi->end.x].knowledge = K_EMPTY;
    mi->robot->knowledgeGrid[mi->end.y][mi->end.x].cost = 0;
    
    mi->robot->knowledgeGrid[mi->start.y][mi->start.x].knowledge = K_EMPTY;
    mi->robot->knowledgeGrid[mi->start.y][mi->start.x].cost = 0;
}

Point point_create(int x, int y) {
    Point p = {.x = x, .y = y};
    return p;
}

int point_equal(const Point p1, const Point p2) {
    return p1.y == p2.y && p1.x == p2.x;
}

char point_move(Point *point, Moves move) {
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