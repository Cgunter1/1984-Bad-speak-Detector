// CITATION: DARRELL LONG PIAZZA: @800

# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _HASH_H
# define _HASH_H
# include <stdint.h>
# include "ll.h"

typedef struct hashTable hashTable; // Sets the variable of struct hashTable to hashTable.

struct hashTable
{
        uint32_t s[4]; // Salt
        uint32_t l;    // Length
        listNode **h;  // Array of pointers to linked lists
};

hashTable *newHT(uint32_t, uint32_t []); // Creates a new hashtable with an array of salts and a lemgth of the hashtable.

void delHT(hashTable *); // Deletes the hashTable with setting all the pointers to NULL.

listNode *findHT(hashTable *, const char *); // Returns to a node of the hashTable, if it is not found, it returns NULL.

void insertHT(hashTable *, const char *, const char *); // Inserts a node into the hashTable with the first and second strings.

uint32_t hash(hashTable *, const char *); // Hash function with hashTable (that includes all the salts) and a certain key.

void printHT(const hashTable *); // Prints out the hashTable.
# endif
