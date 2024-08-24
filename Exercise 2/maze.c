#include "maze.h"
#include "priority_queue/priority_queue.h"
#include <stdlib.h>
#include <assert.h>

PositionBH pbh_create(Point p, int k1, int k2) {
    PositionBH pbh = malloc(sizeof(_PositionBH));
    assert(ppq != NULL);

    pbh->position.x = p.x;
    pbh->position.y = p.y;

    pbh->k1 = k1;
    pbh->k2 = k2;

    return pbh;
}

void pbh_destroy(PositionBH pbh) {
    free(pbh);
}

int pbh_compare(PositionBH pbh_1, PositionBH pbh_2) {
    if(pbh_1->k1 < pbh_2->k1)
        return 1;
    else if(pbh_1->k1 == pbh_2->k1 && pbh_1->k2 <= pbh_2->k2)
        return 0;

    return -1;
}

//Creates and returns a new robot structure.
Robot robot_create() {
    //Assign all the necessary memory and check that it was actually assigned.
    Robot r = malloc(sizeof(_Robot));
    assert(r != NULL);

    r->pq = bheap_create((CompareFunction)pbh_compare, (CopyFunction)pbh_create, (DestroyFunction)pbh_destroy);
    assert(r->pq != NULL);

    r->maze = malloc(sizeof(_Maze));
    assert(r->maze != NULL);
    r->maze->sizeY = 0;
    r->maze->sizeX = 0;
    r->maze->maze = NULL;

    r->start.x = 0;
    r->start.y = 0;
    r->end.x = 0;
    r->end.y = 0;
    r->position.x = 0;
    r->position.y = 0;

    return r;
}

//Destroys the given robot structure.
void robot_destroy(Robot r) {
    //Destroy the priority queue.
    pq_destroy(r->pq);

    //Since "maze" is a double pointer we first free all the memory pointed to by the second pointer.
    for (size_t y = 0; y < r->maze->sizeY; y++)
        free(r->maze->maze[y]);
    free(r->maze);

    free(r);
}

int point_equal(const Point p1, const Point p2) {
    return p1.y == p2.y && p1.x == p2.x;
}