#include "int_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


IntArray intArrayCreate(size_t length) {
    IntArray arr = malloc(sizeof(_IntArray));
    assert(arr != NULL);

    arr->dir = malloc(length * sizeof(int));
    assert(arr->dir != NULL);

    arr->length = length;

    return arr;
}

void intArrayDestroy(IntArray array) {
    free(array->dir);
    free(array);
}

size_t intArrayCapacity(IntArray array) {
    return array->length;
}

int intArrayRead(IntArray array, size_t pos) {
    //Check that the desired position is in the array.
    if (pos > intArrayCapacity(array) - 1) {
        fprintf(stderr, "\nIndex out of array bounds\n");
        exit(-1);
    }

    return array->dir[pos];
}

void intArrayWrite(IntArray array, size_t pos, int value) {
    //If the array isn't long enough resize it so it is.
    if (pos > intArrayCapacity(array) - 1)
        intArrayAdjust(array, (pos + 1) * 2);

    array->dir[pos] = value;
}

void intArrayAdjust(IntArray array, size_t capacity) {
    //We use the length of the shortest array.
    size_t newLenght = capacity < intArrayCapacity(array) ? capacity : intArrayCapacity(array);

    array->dir = realloc(array->dir, newLenght * sizeof(int));
    assert(array->dir != NULL);

    array->length = newLenght;
}


//Inserts the given element at the specified position, moving all elements after it one place to the right, therefore increasing the length of
//the array by one.
/*void intArrayInsert(IntArray array, int pos, int value) {
    //Check that the desired position is in the array.
    if (pos > intArrayCapacity(array) - 1 && pos > 0) {
        fprintf(stderr, "\nIndex out of array bounds\n");
        exit(-1);
    }

    intArrayAdjust(array, intArrayCapacity(array) + 1);

    //Move all elements after the specified position one place to the right. We read them starting from the last one to avoid destroying the
    //values that haven't been copied yet.
    for (int i = intArrayCapacity(array) - 1; i > pos ; i--) {
        intArrayWrite(array, i, intArrayRead(array, i - 1));
    }

    intArrayWrite(array, pos, value);
}


//Deletes the element at the given position and shifts all elements after it one place to the left.
void intArrayDelete(IntArray array, int pos) {
    //Check that the desired position is in the array.
    if (pos > intArrayCapacity(array) - 1 && pos > 0) {
        fprintf(stderr, "\nIndex out of array bounds\n");
        exit(-1);
    }

    for (int i = pos + 1; i < intArrayCapacity(array); i++) {
        intArrayWrite(array, i - 1, intArrayRead(array, i));
    }

    intArrayAdjust(array, intArrayCapacity(array) - 1);
}*/