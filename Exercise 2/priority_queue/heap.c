#include "heap.h"
#include <stdlib.h>
#include <assert.h>


static void _exchange(void **elem1, void **elem2) {
    void *aux = *elem1;
    *elem1 = *elem2;
    *elem2 = aux;
}

BHeap bheap_create(CompareFunction comp, CopyFunction copy, DestroyFunction destr) {
    BHeap heap = malloc(sizeof(_BHeap));
    assert(heap != NULL);

    heap->arr = (void **)malloc(sizeof(void *));
    assert(heap->arr != NULL);

    heap->length = 1;
    heap->last = 0;

    heap->comp = comp;
    heap->copy = copy;
    heap->destr = destr;

    return heap;
}

void bheap_destroy(BHeap heap) {
    for (int i = 1; i <= heap->last; i++)
        heap->destr(heap->arr[i]);

    free(heap);
}

int bheap_empty(BHeap heap) {
    return heap->last == 0;
}

void bheap_travel(BHeap heap, TravelFunction func) {
    for (int i = 1; i <= heap->last; i++)
        func(heap->arr[i]);
}

void bheap_insert(BHeap heap, void *elem) {
    //Check if there's enough space to store the new element, if not double the length of the array.
    if (heap->last >= heap->length - 1) {
        size_t newLength = heap->length * 2;

        heap->arr = realloc(heap->arr, newLength * sizeof(void *));
        assert(heap->arr != NULL);

        heap->length = newLength;
    }

    //Insert new element at the end of the array.
    heap->last++;
    heap->arr[heap->last] = heap->copy(elem);

    int i = heap->last;
    for (; i > 1 && heap->comp(heap->arr[i], heap->arr[i / 2]) > 0; i /= 2)
        _exchange(&heap->arr[i], &heap->arr[i / 2]);
}

//Sinks the element in the given position to it's corresponding position in the heap.
static void _bheap_sink(BHeap heap, int pos, int len) {
    int max_pos = pos;
    int left = 2 * pos;
    int right = 2 * pos + 1;

    //Check if either of the kids are greater than their parent, as long as they are in the heap.
    if (left <= len && heap->comp(heap->arr[left], heap->arr[max_pos]) > 0)
        max_pos = left;
    if (right <= len && heap->comp(heap->arr[right], heap->arr[max_pos]) > 0)
        max_pos = right;

    //If there was a node greater than it's parent we exchange them and make another call with the new position.
    if (max_pos != pos) {
        _exchange(&heap->arr[pos], &heap->arr[max_pos]);
        _bheap_sink(heap, max_pos, len);
    }
}

//Finds the index of the given element in the heap, if it's not present returns -1.
int bheap_find_index(BHeap heap, void *elem) {
    int res = -1;

    for (int i = 1; i < heap->last && res == -1; i++)
        if (heap->comp(heap->arr[i], elem) == 0)
            res = i;

    return res;
}

//Given an index removes that element from the heap.
void bheap_remove_index(BHeap heap, int index) {
    assert(index <= heap->last);

    //Destroy the specified element.
    heap->destr(heap->arr[index]);
    //Copy the last element in the array to the position of the deleted element.
    heap->arr[index] = heap->copy(heap->arr[heap->last]);
    heap->last--;

    //Sink the moved element to it's correct place.
    _bheap_sink(heap, index, heap->last);
}

//Given an element removes it, if present, from the heap.
void bheap_remove(BHeap heap, void *elem) {
    int res = bheap_find_index(heap, elem);

    //The element is not in the heap, we cannot eliminate it.
    if (res == -1)
        return;

    _bheap_remove_index(heap, res);
}

void *bheap_max_elem_pop(BHeap heap) {
    if (bheap_empty(heap))
        return NULL;

    //We get the last element and then remove it from the heap.
    void *elem = heap->copy(heap->arr[1]);
    _bheap_remove_index(heap, 1);

    return elem;
}

void *bheap_max_elem_peek(BHeap heap) {
    if (bheap_empty(heap))
        return NULL;

    return heap->copy(heap->arr[1]);
}