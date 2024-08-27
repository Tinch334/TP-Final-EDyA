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
    //Since "maze" is a double pointer we first free all the memory pointed to by the second pointer.
    for (size_t y = 0; y < mi->maze->sizeY; y++)
        free(mi->maze->maze[y]);
    free(mi->maze->maze);
    free(mi->maze);

    for (size_t y = 0; y < mi->robot->knownHeight; y++)
        free(mi->robot->knowledgeGrid[y]);
    free(mi->robot->knowledgeGrid);
    free(mi->robot);

    free(mi);
}

void initialize_robot(MazeInfo mi, size_t initialWidth, size_t initialHeight) {
    //The robots initial position is the starting position.
    mi->robot->position.y = mi->start.y;
    mi->robot->position.x = mi->start.x;

    //We add one because the position themselves are in the maze.
    mi->robot->knownWidth = initialWidth + 1;
    mi->robot->knownHeight = initialHeight + 1;

    //We create the robots "knowledge gird", used to mark the information the robot has about each cell.
    mi->robot->knowledgeGrid = malloc(mi->robot->knownHeight * sizeof(unsigned int*));
    assert(mi->robot->knowledgeGrid != NULL);

    for (size_t y = 0; y < mi->robot->knownHeight; y++) {
        mi->robot->knowledgeGrid[y] = malloc(mi->robot->knownWidth * sizeof(unsigned int));
        assert(mi->robot->knowledgeGrid[y] != NULL);

        for (size_t x = 0; x < mi->robot->knownWidth; x++)
            mi->robot->knowledgeGrid[y][x] = K_UNKNOWN;
    }

    mi->robot->knowledgeGrid[mi->end.y][mi->end.x] = K_EMPTY;
}

void set_position_knowledge(MazeInfo mi, Point p, unsigned int value) {
    //Check if the grid is too small width wise, if so resize it.
    if (p.x > mi->robot->knownWidth - 1) {
        for (size_t y = 0; y < mi->robot->knownHeight; y++) {
            mi->robot->knowledgeGrid[y] = realloc(mi->robot->knowledgeGrid[y], (p.x + 1) * sizeof(unsigned int));
            assert(mi->robot->knowledgeGrid[y] != NULL);

            for (size_t x = mi->robot->knownWidth - 1; x <= (size_t)p.x; x++)
                mi->robot->knowledgeGrid[y][x] = K_UNKNOWN;
        }

        //Update dimensions.
        mi->robot->knownWidth = p.x + 1;
    }

    //Check if the grid is too small height wise, if so resize it.
    if (p.y > mi->robot->knownHeight - 1) {
        mi->robot->knowledgeGrid = realloc(mi->robot->knowledgeGrid, (p.y + 1) * sizeof(unsigned int*));
        assert(mi->robot->knowledgeGrid != NULL);

        for (size_t y = mi->robot->knownHeight - 1; y <= (size_t)p.y; y++) {
            mi->robot->knowledgeGrid[y] = malloc(mi->robot->knownWidth * sizeof(unsigned int));
            assert(mi->robot->knowledgeGrid[y] != NULL);

            for (size_t x = 0; x < mi->robot->knownWidth; x++)
                mi->robot->knowledgeGrid[y][x] = K_UNKNOWN;
        }

        //Update dimensions.
        mi->robot->knownHeight = p.y + 1;
    }

    mi->robot->knowledgeGrid[p.y][p.x] = value;
}

int get_position_knowledge(MazeInfo mi, Point p) {
    if (p.x >= 0 && p.x < mi->robot->knownWidth && p.y >= 0 && p.y < mi->robot->knownHeight)
        return mi->robot->knowledgeGrid[p.y][p.x] == K_UNKNOWN || mi->robot->knowledgeGrid[p.y][p.x] == K_EMPTY;

    return 1;
}

int valid_position(const MazeInfo mi, const Point p) {
    if (p.x >= 0 && p.x < mi->maze->sizeX && p.y >= 0 && p.y < mi->maze->sizeY)
        return mi->maze->maze[p.y][p.x] == EMPTY;
    else
        return 0;
}

Point point_create(int x, int y) {
    Point p = {.x = x, .y = y};
    return p;
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