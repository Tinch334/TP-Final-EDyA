#include "maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>


//To be used in case the maze file has an incorrect format/data, it deletes the "MazeInfo" structure and prints a corresponding error message.
static void _format_error_handler(MazeInfo mi, char *text, size_t value) {
    fprintf(stderr, "%s %ld\n", text, value);
    mazeinfo_destroy(mi);
    exit(-1);
}

//Takes the maze file, validates it and at the same time creates a "MazeInfo" structure with all the relevant data from the file.
static MazeInfo generate_file(FILE *mazeFile) {
    MazeInfo mi = mazeinfo_create();

    //Read variables in first three lines.
    if (fscanf(mazeFile, "%ld %ld %ld", &mi->maze->sizeY, &mi->maze->sizeX, &mi->sensorRange) != 3)
        _format_error_handler(mi, "Incorrect amount/type of variables, in line", 1);

    if (fscanf(mazeFile, "%d %d", &mi->start->y, &mi->start->x) != 2)
        _format_error_handler(mi, "Incorrect amount/type of variables, in line", 2);

    if (fscanf(mazeFile, "%d %d", &mi->end->y, &mi->end->x) != 2) 
        _format_error_handler(mi, "Incorrect amount/type of variables, in line", 3);

    size_t mazeSizeY = mi->maze->sizeY;
    size_t mazeSizeX = mi->maze->sizeX;

    //If the first three lines were read correctly we can create the maze and start reading it.
    mi->maze->maze = malloc(mazeSizeY * sizeof(unsigned int*));
    assert(mi->maze->maze != NULL);

    for (size_t y = 0; y < mazeSizeY; y++) {
        mi->maze->maze[y] = malloc(mazeSizeX * sizeof(unsigned int));
        assert(mi->maze->maze[y] != NULL);
    }

    char c;
    size_t characterCounter = 0;
    size_t lineCounter = 0;

    //We read the character/s left in the third line, they can either be a newline, or a carriage return followed by a newline(On Windows).
    c = getc(mazeFile);
    if (c == '\r') {
        c = getc(mazeFile);
        if (c != '\n')
            _format_error_handler(mi, "Incorrect amount/type of variables, in line", 3);
    }
    else if (c != '\n')
        _format_error_handler(mi, "Incorrect amount/type of variables, in line", 3);

    while ((c = getc(mazeFile)) != EOF) {
        //Check if there are more than the allowed amount of characters in a line.
        if (characterCounter > mazeSizeX)
            _format_error_handler(mi, "Too many characters, in line", lineCounter + 4);

        //If the character is a valid maze character add a 1 to the array if it's a #, otherwise add a 0.
        if (c == '.')
            mi->maze->maze[lineCounter][characterCounter++] = EMPTY;
        else if (c == '#')
            mi->maze->maze[lineCounter][characterCounter++] = WALL;
        //Whenever a newline is reached check that the amount of read lines and characters is correct.
        else if (c == '\n')
            if (characterCounter != mazeSizeX)
                _format_error_handler(mi, "Too few characters, in line ", lineCounter + 4);
            else {
                characterCounter = 0;
                lineCounter++;

                if (lineCounter > mazeSizeY) {
                    puts("Too many lines");
                    _format_error_handler(mi, "Too many maze lines, correct number is", mazeSizeY);
                }
            }
        //This allows files with Windows line endings to be read.
        else if (c != '\r')
            _format_error_handler(mi, "Invalid maze character, in line", lineCounter + 4);
    }

    //Check that the specified number of lines was read. We subtract one to the Y size because "lineCounter" starts at zero.
    if (lineCounter < mazeSizeY - 1)
        _format_error_handler(mi, "Too many maze lines, correct number is", mazeSizeY);

    //Check that the start and end tiles are empty.
    if (mi->maze->maze[mi->start->y][mi->start->x] != 0)
        _format_error_handler(mi, "Start must be an empty tile, in line", mi->start->y);
    if (mi->maze->maze[mi->end->y][mi->end->x] != 0)
        _format_error_handler(mi, "End must be an empty tile, in line", mi->end->y);

    //The robots starting position is specified in the file.
    mi->position->y = mi->start->y;
    mi->position->x = mi->start->x;
    
    return mi;
}

/*
    CHECK SENSOR USAGE
*/
int main(int argc, char const *argv[])
{
    //Check that a filename was supplied.
    if (argc != 2) {
        fprintf(stderr, "Missing filename - Usage: ./firstSolver <file>\n");
        exit(-1);
    }

    //Check that the file was opened successfully.
    FILE *mazeFile = fopen(argv[1], "r");
    if (mazeFile == NULL) {
        fprintf(stderr, "%s: This file doesn't exist or you do not have permission to read it\n", argv[1]);
        exit(-1);
    }

    //Get maze info.
    MazeInfo mi = generate_file(mazeFile);

    for (size_t i = 0; i < mi->maze->sizeY; i++) {
        for (size_t j = 0; j < mi->maze->sizeX; j++)
            fprintf(stdout, "%d", mi->maze->maze[i][j]);

        puts("");
    }

    //Seed the random number generator.
    srand(time(NULL));

    mazeinfo_destroy(mi);
    fclose(mazeFile);

    return 0;
}