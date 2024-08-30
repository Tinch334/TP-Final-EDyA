#ifndef __GLIST_H__
#define __GLIST_H__

typedef void (*DestroyFunction)(void *data);
typedef void *(*CopyFunction)(void *data);
typedef void (*VisitorFunction)(void *data);
//Returns a negative value if data1 < data2, 0 if they are equal and a positive value if data1 > data2.
typedef int (*CompareFunction)(void *data1, void *data2);

typedef struct _GNode {
    void *data;
    struct _GNode *next;
} GNode;

typedef GNode *GList;

//Returns an empty list.
GList glist_create();

//Destroys the given list.
void glist_destroy(GList list, DestroyFunction destroyFunc);

//Returns 1 if the given list is empty.
int glist_empty(GList list);

//Adds the given element to the start of the list.
GList glist_add_start(GList list, void *data, CopyFunction copyFunc);

//Adds the given element to the end of the list.
GList glist_add_end(GList list, void *data, CopyFunction copyFunc);

//Travels the list, calling the visitor function with each element.
void glist_travel(GList list, VisitorFunction visitorFunc);

//Returns the element at the start of the list, if it's empty returns NULL.
void *glist_get_start(GList list, CopyFunction copyFunc);

//Destroys the first element in the list.
GList glist_delete_start(GList list, DestroyFunction destroyFunc);

//Returns 1 if the given value is in the list, returns 0 otherwise.
int glist_find(GList list, CompareFunction comp, void *data);

#endif/* __GLIST_H__ */