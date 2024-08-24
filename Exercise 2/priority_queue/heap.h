#ifndef __HEAP_H__
#define __HEAP_H__


typedef int (*CompareFunction) (void *elem1, void *elem2);
typedef void *(*CopyFunction) (void *elem);
typedef void (*DestroyFunction) (void *elem);
typedef void (*TravelFunction) (void *elem);


typedef struct {
    void **arr;
    int length;
    int last;
    CompareFunction comp;
    CopyFunction copy;
    DestroyFunction destr;
} _BHeap;

typedef _BHeap *BHeap;


BHeap bheap_create(CompareFunction comp, CopyFunction copy, DestroyFunction destr);

void bheap_destroy(BHeap heap);

int bheap_empty(BHeap heap);

void bheap_travel(BHeap heap, TravelFunction func);

void bheap_insert(BHeap heap, void *elem);

void bheap_remove(BHeap heap, void *elem);

int bheap_find_index(BHeap heap, void *elem);

void bheap_remove_index(BHeap heap, int index);

void *bheap_max_elem_pop(BHeap heap);

void *bheap_max_elem_peek(BHeap heap);


#endif /* __HEAP_H__ */