#include "priority_queue.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


PriorityQueue pq_create(CompareFunction comp, CopyFunction copy, DestroyFunction destr) {
    return bheap_create(comp, copy, destr);
}

int pq_empty(PriorityQueue pq) {
    return bheap_empty(pq);
}

void *pq_peek_max(PriorityQueue pq) {
    return bheap_max_elem_peek(pq);
}

void pq_pop_max(PriorityQueue pq) {
    void *res = bheap_max_elem_pop(pq);
    //We are only interested in deleting the element, not returning it.
    pq->destr(res);
}

void pq_insert(PriorityQueue pq, void *elem) {
    bheap_insert(pq, elem);
}