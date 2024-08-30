#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "gdlist.h"

typedef GDList Queue;

//Creates and returns an empty stack.
Queue queue_create();

//Destroys the given stack.
void queue_destroy(Queue q, DestroyFunction destroyFunc);

//Returns 1 if the given stack is empty, 0 otherwise.
int queue_empty(Queue q);

//Enqueues the given element in the queue.
void queue_enqueue(Queue *q, CopyFunction copy, void *value);

//Dequeues the top element from the stack, if the stack is empty returns NULL.
void *queue_dequeue(Queue *q, DestroyFunction destroyFunc, CopyFunction copyFunc);

#endif/* __QUEUE_H__ */