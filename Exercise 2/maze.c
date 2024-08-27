#include "maze.h"
#include <stdlib.h>
#include <assert.h>


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

    //We create the robots "knowledge gird", used to mark the information the robot has about each cell.
    mi->robot->knowledgeGrid = malloc(mi->robot->mazeSizeY * sizeof(MazeCell*));
    assert(mi->robot->knowledgeGrid != NULL);

    for (size_t y = 0; y < mi->robot->mazeSizeY; y++) {
        mi->robot->knowledgeGrid[y] = malloc(mi->robot->mazeSizeX * sizeof(MazeCell));
        assert(mi->robot->knowledgeGrid[y] != NULL);

        for (size_t x = 0; x < mi->robot->mazeSizeX; x++) {
            mi->robot->knowledgeGrid[y][x].knowledge = K_UNKNOWN;
            mi->robot->knowledgeGrid[y][x].g = inf(mi);
            mi->robot->knowledgeGrid[y][x].h = inf(mi);
        }
    }

    //We know the end is empty.
    mi->robot->knowledgeGrid[mi->end.y][mi->end.x].knowledge = K_EMPTY;
    mi->robot->knowledgeGrid[mi->end.y][mi->end.x].h = 0;
    //The cost of the start is zero.
    mi->robot->knowledgeGrid[mi->start.y][mi->start.x].knowledge = K_EMPTY;
    mi->robot->knowledgeGrid[mi->start.y][mi->start.x].h = 0;
    mi->robot->knowledgeGrid[mi->start.y][mi->start.x].g = 0;
}

int inf(MazeInfo mi) {
    return mi->robot->mazeSizeX * mi->robot->mazeSizeY + 1;
}

Point point_create(int x, int y) {
    Point p = {.x = x, .y = y};
    return p;
}

int point_equal(const Point p1, const Point p2) {
    return p1.y == p2.y && p1.x == p2.x;
}