#ifndef __STACK_H__
#define __STACK_H__
#include "glist.h"

typedef GList Stack;

//Creates and returns an empty stack.
Stack stack_create();

//Destroys the given stack.
void stack_destroy(Stack s, DestroyFunction destroyFunc);

//Returns 1 if the given stack is empty, 0 otherwise.
int stack_empty(Stack s);

//Returns the top element of the stack without popping it.
void *stack_peek(Stack s, CopyFunction copy);

//Pushes the given element to the stack.
void stack_push(Stack *s, CopyFunction copy, void *value);

//Pops the top element from the stack, if the stack is empty returns NULL.
void *stack_pop(Stack *s, DestroyFunction destroyFunc, CopyFunction copyFunc);

//Prints the stack.
void stack_print(Stack s, VisitorFunction printFunc);


#endif/* __STACK_H__ */