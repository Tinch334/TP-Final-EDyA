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

GList glist_add_start(GList list, void *data, CopyFunction copyFunc) {
    GNode *newNode = malloc(sizeof(GNode));
    assert(newNode != NULL);

    newNode->next = list;
    newNode->data = copyFunc(data);

    return newNode;
}

void *glist_get_start(GList list, CopyFunction copyFunc) {
    if (glist_empty(list))
        return NULL;

    return copyFunc(list->data);
}

GList glist_delete_start(GList list, DestroyFunction destroyFunc) {
    if (glist_empty(list))
        return NULL;

    GList next = list->next;

    destroyFunc(list->data);
    free(list);

    return next;
}

int glist_find(GList list, CompareFunction compFunc, void *data) {
    int found = 0;

    for (GNode *node = list; node != NULL; node = node->next)
        if (compFunc(node->data, data) == 0)
            found = 1;

    return found;
}

void glist_travel_extra(GList list, VisitorExtraFunction visitorFunc, void *extraValue) {
    for (GNode *node = list; node != NULL; node = node->next)
        visitorFunc(extraValue, list->data);
}