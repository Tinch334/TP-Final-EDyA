#include "glist.h"
#include <assert.h>
#include <stdlib.h>


GList glist_create() {
    return NULL;
}

void glist_destroy(GList list, DestroyFunction destroyFunc) {
    GNode *nodeToDelete;
    while (list != NULL) {
        nodeToDelete = list;
        list = list->next;
        destroyFunc(nodeToDelete->data);
        free(nodeToDelete);
    }
}

int glist_empty(GList list) {
    return (list == NULL);
}

GList glist_add_start(GList list, void *data, CopyFunction copy) {
    GNode *newNode = malloc(sizeof(GNode));
    assert(newNode != NULL);

    newNode->next = list;
    newNode->data = copy(data);

    return newNode;
}

GList glist_add_end(GList list, void *data, CopyFunction copy) {
    GNode *newNode = malloc(sizeof(GNode));
    assert(newNode != NULL);
    newNode->next = NULL;
    newNode->data = copy(data);

    //If the list was empty return the new start.
    if (list == NULL)
        return newNode;

    GNode *node;
    for (node = list; node->next != NULL; node = node->next);
    node->next = newNode;

    return list;
}

void glist_travel(GList list, VisitorFunction visit) {
    for (GNode *node = list; node != NULL; node = node->next)
        visit(node->data);
}


void *glist_get_start(GList list, CopyFunction copyFunc) {
    if (glist_empty(list))
        return NULL;

    return copyFunc(list->data);
}

GList glist_delete_start(GList list, DestroyFunction destroyFunc) {
    if (glist_empty(list))
        return NULL;

    else {
        GList next = list->next;

        destroyFunc(list->data);
        free(list);

        return next;
    }
}

int glist_find(GList list, CompareFunction comp, void *data) {
    int found = 0;

    for (GNode *node = list; node != NULL; node = node->next)
        if (comp(node->data, data) == 0)
            found = 1;

    return found;
}