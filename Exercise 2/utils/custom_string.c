#include "custom_string.h"
#include <stdlib.h>
#include <assert.h>


CString cstring_create(size_t initialSize) {
    assert(initialSize > 0);

    CString cstring = malloc(sizeof(_CString));
    assert(cstring != NULL);

    cstring->string = '\0';
    cstring->size = initialSize;
    cstring->length = 0;

    return cstring;
}

void cstring_add_char(CString cstring, char c) {
    (cstring->length)++;

    if (cstring->length - 1 > cstring->size) {
        cstring->size = cstring->size * 2;
        cstring->string = realloc(cstring->string, sizeof(char) * cstring->size);
    }

    cstring->string[cstring->length - 1] = c;
    cstring->string[cstring->length] = '\0';
}

void cstring_destroy(CString cstring) {
    free(cstring->string);
    free(cstring);
}