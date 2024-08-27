#ifndef __INT_ARRAY_H__
#define __INT_ARRAY_H__
#include <stdlib.h>


typedef struct {
    int *dir;
    size_t length;
} _IntArray;
typedef _IntArray *IntArray;


//Creates an array with the specified length and returns it.
IntArray intArrayCreate(size_t length);

//Destroys the given array.
void intArrayDestroy(IntArray array);

//Returns the length of the array.
size_t intArrayCapacity(IntArray array);

//Reads a value in the specified position from the array.
int intArrayRead(IntArray array, size_t pos);

//Writes the given value in the specified position in the array, if there's not enough space
//the array is resized to accommodate the new position.
void intArrayWrite(IntArray array, size_t pos, int value);

//Resizes the given array, if the new length is smaller than the current one the array gets truncated.
void intArrayAdjust(IntArray array, size_t capacity);

/*void intArrayInsert(IntArray array, int pos, int value);
void intArrayDelete(IntArray array, int pos);*/

#endif /* __INT_ARRAY_H__ */