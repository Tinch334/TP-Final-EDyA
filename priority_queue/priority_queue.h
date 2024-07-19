#include "heap.h"

#ifndef __PQ_H__
#define __PQ_H__

typedef BHeap PriorityQueue;


PriorityQueue pq_create(CompareFunction comp, CopyFunction copy, DestroyFunction destr);

int pq_empty(PriorityQueue pq);

void *pq_peek_max(PriorityQueue pq);

void pq_pop_max(PriorityQueue pq);

void pq_insert(PriorityQueue pq, void *elem);


#endif /* __PQ_H__ */