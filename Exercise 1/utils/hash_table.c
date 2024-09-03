#include "hash_table.h"
#include "glist.h"
#include <assert.h>
#include <stdlib.h>


HashTable htable_create(size_t size, CopyFunctionHash copy, CompareFunctionHash comp, DestroyFunctionHash destroy, HashFunction hash) {
    HashTable table = malloc(sizeof(_HashTable));
    assert(table != NULL);

    table->elems = malloc(sizeof(GList) * size);
    assert(table->elems != NULL);

    table->elemCount = 0;
    table->size = size;
    table->copy = copy;
    table->comp = comp;
    table->destroy = destroy;
    table->hash = hash;

    //Create an empty list in all the cells.
    for (size_t i = 0; i < table->size; i++)
        table->elems[i] = glist_create();

    return table;
}

size_t htable_elem_count(HashTable table) {
    return table->elemCount;
}

size_t htable_size(HashTable table) {
    return table->size;
}

void htable_destroy(HashTable table) {
    for (size_t i = 0; i < table->size; i++)
        glist_destroy(table->elems[i], table->destroy);

    //Free the structure.
    free(table->elems);
    free(table);
}

//Resizes the given hash table to double it's current size.
static void htable_resize(HashTable table) {
    //We maintain a reference to the old elements so they can be properly deleted.
    GList *oldElems = table->elems;
    size_t oldSize = table->size;

    table->size = oldSize * 2;
    table->elems = malloc(sizeof(GList) * table->size);

    //Create an empty list in all the cells.
    for (size_t i = 0; i < table->size; i++)
        table->elems[i] = glist_create();

    //Add elements in each list to new list.
    for (size_t i = 0; i < oldSize; i++)
        glist_travel_extra(oldElems[i], (VisitorExtraFunction)htable_insert, table);

    //Destroy old elements.
    for (size_t i = 0; i < oldSize; i++)
        glist_destroy(oldElems[i], table->destroy);

    free(oldElems);
}

void htable_insert(HashTable table, void *data) {
    //Get the data's position according to the hash function.
    size_t i = table->hash(data) % table->size;
    table->elemCount++;

    //Add the element to it's corresponding list.
    table->elems[i] = glist_add_start(table->elems[i], data, table->copy);

    //If the load factor gets too high resize the table.
    if ((float)table->elemCount / (float)table->size >= (float)MAX_LOAD_FACTOR)
        htable_resize(table);
}

int htable_contains(HashTable table, void *data) {
    //Get the data's position according to the hash function.
    size_t i = table->hash(data) % table->size;

    //If the list in the cell was empty return the element isn't in the table.
    if (glist_empty(table->elems[i]))
      return 0;
    // Retornar el data de la casilla si hay concidencia.
    else 
        return glist_find(table->elems[i], table->comp, data);
}