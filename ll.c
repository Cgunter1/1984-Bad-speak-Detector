// CITATION: ARRAYLINKEDLIST.C TUTORIAL ON PIAZZA.
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include "ll.h"


listNode *newNode(const char *old, const char *new)
{
	listNode *node; // Creates a node.
	node = malloc(sizeof(listNode)); // Sets aside memory for that node.
	// Creates strings that will hold each word, old and new.
	char *o = (char *)malloc(1000 * sizeof(char)); 
	char *n = (char *)malloc(1000 * sizeof(char));
	// Copies the old string and new string to o and n variables.
	strcpy(o, old);
	strcpy(n, new);
	node->oldspeak = o;
	node->newspeak = n;
	node->next = NIL;
	return node; // Returns node with its own 
}

void delNode(listNode *node) 
// Deletes each node's oldspeak, newspeak, and then itself. Returns nothing.
{
	// Deletes the oldspeak and newspeak and sets the pointers to NULL.
	free(node->oldspeak);
	node->oldspeak = NULL;
	free(node->newspeak);
	node->newspeak = NULL;
	node->next = NULL;
	free(node);
	return;
}

void delLL(listNode *node) 
// Deletes the whole linked list by going through it and 
// deleting every node and going forward.
{
	listNode *temp;
	while(node != NULL)
	{
		temp = node;
		node = node->next;
		delNode(temp);
	}
	return;
}

// Return node of the head node.
listNode *insertLL(listNode **head, const char *old, const char *new)
{
	char *n;
	listNode *insertNode; 
	// Searches if there is a similar node that already exists.
	if(*head == NULL)
	{
		*head = newNode(old, new);
		return *head;
	}
	else
	{
		insertNode = findLL(head, old);
		if(insertNode != NULL) 
		// If there is node that exists with the oldspeak. 
		// It replaces the newspeak word with another word.
		{
			n = (char *)malloc(1000 * sizeof(char));
			strcpy(n, new);
			free(insertNode->newspeak);
			insertNode->newspeak = n;
		}
		else
	// Create a node for that oldspeak and newspeak pair.
		{
		insertNode = newNode(old, new);
		insertNode->next = (*head);
		}
	}
	// Then return that node.
	return insertNode;
}
// Will return node if found or NULL if not found.
listNode *findLL(listNode **head, const char *oldCheck)
{
	listNode *finder = (*head);
	listNode *prev;
	uint8_t found = 0;
	uint32_t behindHead = 0;
	++wordLinks;
	++seeks;
	while(finder != NULL && (found == 0)) // Checks if the finder is NULL or if the node is found.
	{
		++seeks;
		if(strcmp(finder->oldspeak, oldCheck) == 0)
		{
			if(behindHead > 0) 
                	{
				printf("\nk\n");
	                        prev->next = finder->next;
        	                finder->next = (*head);
                	        (*head) = finder;
        	        }
			// Then found is added to one to end the while-loop.
			++found;
			return finder;
		}
		if(found == 0)
		{
			prev = finder;
			finder = finder->next;
			++behindHead;
		}
	}
	return NULL;
}

void printLL(listNode *node) // Simply prints out the linked list.
{
	listNode *temp = node;
	while(temp != NULL)
	{
		printf("%s -> %s \n", temp->oldspeak, temp->newspeak);
		temp = temp->next;
	}
	return;
}
