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
    scanf("%d%d%d", &sizeY, &sizeX, &sensorRange);
    scanf("%d%d", &startY, &startX);
    scanf("%d%d", &endY, &endX);

    //Seed the random number generator.
    srand(time(NULL));

    MazeInfo mi = mazeinfo_create(point_create(startX, startY), point_create(endX, endY), sizeX, sizeY);

    sensor_solver(mi);
    
    mazeinfo_destroy(mi);

    return 0;
}