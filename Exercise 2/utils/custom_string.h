#ifndef __CUSTOM_STRING_H__
#define __CUSTOM_STRING_H__
#include <stdlib.h>

typedef struct {
    char *string;
    size_t ptr;
    size_t size;
} _CString;
typedef _CString *CString;

//Creates an empty cstring with the specified size.
CString cstring_create(size_t initialSize);

//Adds the given character to the cstring.
void cstring_add_char(CString cstring, char c);

//Destroys the given cstring.
void cstring_destroy(CString cstring);

#endif /* __CUSTOM_STRING_H__ */