#include "maze.h"
#include "second_solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>


int main(int argc, char const *argv[])
{
    int startX, startY, endX, endY, sizeX, sizeY, sensorRange;

    //Read all the information from "sensor.c".
    scanf("%d%d%d", &sizeX, &sizeY, &sensorRange);
    scanf("%d%d", &startX, &startY);
    scanf("%d%d", &endX, &endY);

    //Seed the random number generator.
    srand(time(NULL));

    Robot r = robot_create();
    r->start.x = startX;
    r->start.y = startY;
    r->end.x = endX;
    r->end.y = endY;
    r->maze->sizeX = sizeX;
    r->maze->sizeY = sizeY;
    r->sensorRange = sensorRange;

    sensor_solver(r);

    robot_destroy(r);

    printf("! L\n");

    return 0;
}