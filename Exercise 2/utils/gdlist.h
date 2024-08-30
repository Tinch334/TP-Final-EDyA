#ifndef __GDLIST_H__
#define __GDLIST_H__

typedef void (*DestroyFunction)(void *data);
typedef void *(*CopyFunction)(void *data);

typedef struct _GNode {
    void *data;
    struct _GNode *next;
    struct _GNode *prev;
} GDNode;

typedef struct {
    GDNode *start;
    GDNode *end;
} _GDList;

typedef _GDList *GDList;

//Returns an empty list.
GDList gdlist_create();

//Destroys the given list.
void gdlist_destroy(GDList list, DestroyFunction destroyFunc);

//Returns 1 if the given list is empty.
int gdlist_empty(GDList list) ;

//Adds the given element to the start of the list.
GDList gdlist_add_start(GDList list, void *data, CopyFunction copyFunc);

//Returns the element at the end of the list, if it's empty returns NULL.
void *gdlist_get_end(GDList list, CopyFunction copyFunc);

//Destroys the last element of the list.
GDList gdlist_delete_end(GDList list, DestroyFunction destroyFunc);

#endif/* __GDLIST_H__ */