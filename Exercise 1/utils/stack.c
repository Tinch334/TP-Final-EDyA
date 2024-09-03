#include "stack.h"


Stack stack_create() {
    return glist_create();
}

void stack_destroy(Stack s, DestroyFunction destroyFunc) {
    glist_destroy(s, destroyFunc);
}

int stack_empty(Stack s) {
    return glist_empty(s);
}

void *stack_peek(Stack s, CopyFunction copyFunc) {
    return glist_get_start(s, copyFunc);
}

void stack_push(Stack *s, CopyFunction copyFunc, void *value) {
    *s = glist_add_start(*s, value, copyFunc);
}

void *stack_pop(Stack *s, DestroyFunction destroyFunc, CopyFunction copyFunc) {
    void *value = glist_get_start(*s, copyFunc);
    *s = glist_delete_start(*s, destroyFunc);

    return value;
}