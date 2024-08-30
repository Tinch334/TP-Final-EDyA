#include "queue.h"
#include <stdlib.h>
#include <stdio.h>


Queue queue_create() {
    return gdlist_create();
}

void queue_destroy(Queue q, DestroyFunction destroyFunc) {
    gdlist_destroy(q, destroyFunc);
}

int queue_empty(Queue q) {
    return gdlist_empty(q);
}

void queue_enqueue(Queue *q, CopyFunction copyFunc, void *value) {
    *q = gdlist_add_start(*q, value, copyFunc);
}

void *queue_dequeue(Queue *q, DestroyFunction destroyFunc, CopyFunction copyFunc) {
    if (gdlist_empty(*q))
        return NULL;

    void *value = gdlist_get_end(*q, copyFunc);
    printf("Inner, got: %d\n", *((int *)value));
    *q = gdlist_delete_end(*q, destroyFunc);

    return value;
}