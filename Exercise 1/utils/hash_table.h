#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#include "hash_table.h"
#include "glist.h"
#include <stdlib.h>


typedef void *(*CopyFunctionHash)(void *data);
//Returns a negative value if data1 < data2, 0 if they are equal and a positive value if data1 > data2.
typedef int (*CompareFunctionHash)(void *data1, void *data2);
typedef void (*DestroyFunctionHash)(void *data);
typedef unsigned (*HashFunction)(void *data);

//Note that this hash table implementation utilizes chaining to deal with collisions.
typedef struct {
    GList *elems;
    size_t elemCount;
    size_t size;

    CopyFunctionHash copy;
    CompareFunctionHash comp;
    DestroyFunctionHash destroy;
    HashFunction hash;
} _HashTable;
typedef _HashTable *HashTable;

//Creates a new hash table.
HashTable htable_create(size_t size, CopyFunctionHash copy, CompareFunctionHash comp, DestroyFunctionHash destroy, HashFunction hash);

//Returns the amount of elements in the table.
size_t htable_elem_count(HashTable table);

//Returns the size of the table
size_t htable_size(HashTable table);

//Destroys the given hash table.
void htable_destroy(HashTable table);

//Inserts the given element into the table.
void htable_insert(HashTable table, void *data);

//Returns 1 if the given element is present in the table, 0 otherwise.
int htable_contains(HashTable table, void *data);

#endif /* __HASH_TABLE_H__ */