# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef MTF
# define MTF	true
# endif

# ifndef _LL_H
# define _LL_H

# include <string.h>
# include <stdbool.h>
# include <stdio.h>

extern bool moveToFront; // Changes whether moveToFront is true.

extern uint32_t seeks; // Finds all the seeks.

extern double links; 

extern uint32_t wordLinks; // Determines the number of words given to the linked list.

typedef struct listNode listNode; // Variable of listNode = struct listNode.

struct listNode 
// Creates a struct equal to the strings of oldspeak, newspeak, and the link to the next node.
{
	char *oldspeak, *newspeak;
	listNode *next;	
};

listNode *newNode(const char *, const char *); // Creates a newNode for the linked list.

void delNode(listNode *); // Deletes a specific node.

void delLL(listNode *); // Deletes the whole linked list.

listNode *insertLL(listNode **, const char *, const char *); 
// Takes oldspeak and newspeak words and inserts them into a linked list.

listNode *findLL(listNode **, const char *); 
// Takes the oldspeak and searchs the linked list for that node that contains it.

void printLL(listNode *);
// Prints the whole linked list.
# endif
