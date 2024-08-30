#include "gdlist.h"
#include <assert.h>
#include <stdlib.h>


GDList gdlist_create() {
    GDList list = malloc(sizeof(_GDList));
    assert(list != NULL);

    list->start = NULL;
    list->end = NULL;

    return list;
}

void gdlist_destroy(GDList list, DestroyFunction destroyFunc) {
    GDNode *node = list->start;
    GDNode *nodeToDelete;

    while (node != NULL) {
        nodeToDelete = node;
        node = node->next;
        destroyFunc(nodeToDelete->data);
        free(nodeToDelete);
    }

    free(list);
}

int gdlist_empty(GDList list) {
    return list->start == NULL;
}

GDList gdlist_add_start(GDList list, void *data, CopyFunction copyFunc) {
    GDNode *newNode = malloc(sizeof(GDNode));
    assert(newNode != NULL);

    //This works even if the list is empty, since it will just be "NULL".
    newNode->next = list->start;
    newNode->data = copyFunc(data);

    //If the list's not empty set the pointer of the element after this one.
    if (list->start != NULL)
        list->start->prev = newNode;

    //If the list's empty the last node is the same as the first node.
    if (list->end == NULL)
        list->end = newNode;

    //We set the previous node now in case the list was empty.
    newNode->prev = list->end;

    list->start = newNode;

    return list;
}

void *gdlist_get_end(GDList list, CopyFunction copyFunc) {
    if (gdlist_empty(list))
        return NULL;

    return copyFunc(list->end->data);
}

GDList gdlist_delete_end(GDList list, DestroyFunction destroyFunc) {
    if (gdlist_empty(list))
        return NULL;

    GDNode *nodeToDelete = list->end;

    //If the list only has one element we must set both references to NULL, since it will become empty.
    if (list->start == list->end) {
        list->start = NULL;
        list->end = NULL;
    }
    else {
        list->end = list->end->prev;
        //The last element has the start of the list as it's next element, the list is circular.
        list->end->next = list->start;
    }

    destroyFunc(nodeToDelete->data);
    free(nodeToDelete);

    return list;
}