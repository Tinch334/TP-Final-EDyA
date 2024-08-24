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

void *stack_peek(Stack s, CopyFunction copy) {
    return glist_get_start(s, copy);
}

void stack_push(Stack *s, CopyFunction copy, void *value) {
    *s = glist_add_start(*s, value, copy);
}

void *stack_pop(Stack *s, DestroyFunction destroyFunc, CopyFunction copy) {
    void *value = glist_get_start(*s, copy);
    *s = glist_delete_start(*s, destroyFunc);

    return value;
}

void stack_print(Stack s, VisitorFunction printingFunction) {
    glist_travel(s, printingFunction);
}