#include "maze.h"
#include "utils/hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


//The functions hash the points using a basic hashing algorithm, the constants where chosen so that points could have values of up to
//1000 in each component.
static int hash_int(int key)
{
    return ((11 * key) % 75);
}

static int hash_point(Point *p) {
    return (53 + hash_int(p->x)) * 53 + hash_int(p->y);
}

//The following three functions are used to handle the points in the hash table.
static Point *p_point_copy(Point *p) {
    Point *newP = malloc(sizeof(Point));
    newP->x = p->x;
    newP->y = p->y;

    return newP;
}

static int p_point_equal(Point *p1, Point *p2) {
    return abs(p1->y - p2->y) + abs(p1->x - p2->x);
}

static void p_point_destroy(Point *p1) {
    free(p1);
}

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
    mi->robot->knowledgeTable = NULL;

    return mi;
}

void mazeinfo_destroy(MazeInfo mi) {
    //Since "maze" is a double pointer we first free all the memory pointed to by the second pointer.
    for (size_t y = 0; y < mi->maze->sizeY; y++)
        free(mi->maze->maze[y]);
    free(mi->maze->maze);
    free(mi->maze);

    //Destroy the robot's hash table.
    htable_destroy(mi->robot->knowledgeTable);

    free(mi);
}

void initialize_robot(MazeInfo mi, size_t initialWidth, size_t initialHeight) {
    mi->robot->position.x = mi->start.x;
    mi->robot->position.y = mi->start.y;

    //We want the size of the hash table to be high to keep the load factor low.
    size_t htable_size = 100 < initialWidth * initialHeight ? initialWidth * initialHeight : 100;

    mi->robot->knowledgeTable = htable_create(htable_size, (CopyFunctionHash)p_point_copy, (CompareFunctionHash)p_point_equal, (DestroyFunctionHash)p_point_destroy, (HashFunction)hash_point);
}

void mark_position(MazeInfo mi, Point *p) {
    htable_insert(mi->robot->knowledgeTable, p);
}

int get_position(MazeInfo mi, Point *p) {
    return htable_contains(mi->robot->knowledgeTable, p);
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