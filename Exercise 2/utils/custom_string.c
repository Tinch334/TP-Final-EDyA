#include "custom_string.h"
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>


CString cstring_create(size_t initialSize) {
    assert(initialSize > 0);

    CString cstring = malloc(sizeof(_CString));
    assert(cstring != NULL);

    cstring->string = '\0';
    cstring->size = initialSize;
    cstring->ptr = 0;

    return cstring;
}

void cstring_add_char(CString cstring, char c) {
    (cstring->size)++;

    //Check if we've run out of space.
    if (cstring->ptr - 1 >= cstring->size) {
        cstring->size = cstring->size * 2;
        cstring->string = realloc(cstring->string, sizeof(char) * cstring->size);

        assert(cstring->string != NULL);
    }

    cstring->string[cstring->ptr] = c;
    (cstring->ptr)++;
    cstring->string[cstring->ptr] = '\0';
}

void cstring_destroy(CString cstring) {
    free(cstring->string);
    free(cstring);
}