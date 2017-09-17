// CITATION: DARRELL LONG PIAZZA: @800

# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "hash.h"
# include "aes.h"
# include "bf.h"
# include "ll.h"


static inline int realLength(int l)
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

uint32_t hash(hashTable *h, const char *key)
{
	uint32_t output[4] = { 0x0 };
	uint32_t sum       = 0x0;
	int keyL           = strlen(key);
	uint8_t *realKey   = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t
));

	memcpy(realKey, key, keyL);

	for (int i = 0; i < realLength(keyL); i += 16)
	{
		AES128_ECB_encrypt((uint8_t *) h->s, 	        // Salt
                                   (uint8_t *) realKey + i,	// Input
				   (uint8_t *) output);	        // Output
		sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
	}
	free(realKey);
	return sum;
}

hashTable *newHT(uint32_t length, uint32_t seed[])
{
	hashTable *newHash = (hashTable *) malloc(sizeof(hashTable)); // Sets aside space for the hashTable of the size of the hashTable.
	for(uint8_t i = 0; i < 4; i++) // Sets the sailt array that is within the arguments.
	{
		newHash->s[i] = seed[i];
	}
	newHash->l = length;
	newHash->h = (listNode **) calloc(length, sizeof(listNode)); // Sets aside h as an array of pointers to linked lists.
	return newHash; // Returns a new hashTable.
}

void delHT(hashTable *ht) //Deletes hashTable.
{
	for(uint32_t index = 0; index <= ht -> l; index++)
	{
		if(ht -> h[index] != NULL) // Goes through the whole h-array by going until it is NULL.
		{
			delLL(ht->h[index]); // For each header, it frees all the linked lists for that node on the h-array.
		}
	}
	free(ht->h);  // Frees the array of linked lists.
	ht->h = NULL; // Sets the pointer to NULL.
	free(ht); // Frees the hashTable.
	return;
}

listNode *findHT(hashTable *ht, const char *old) // Finds a specific part in the array that are found using the old keyword.
{
	uint32_t hashedWord;
	listNode *selected;
	//char *word = (char *) malloc(sizeof(char) * 1000);
	//strcpy(word, old);
	hashedWord = hash(ht, old); // finds the hash number of the word given and the 
	hashedWord %= (ht->l); // Do the modulo to find the index within the range of the array.
	listNode **exactNode = &(ht -> h[hashedWord]); // This sets a pointer to the node of the linked list.
	selected = findLL(exactNode, old); // Finds the word in the hashTable.
	return selected; // Returns a node if found or NULL if not.
}

void insertHT(hashTable *ht, const char *old, const char *new) // Inserts hashtable into the table with
{
	uint32_t hashedWord;
        hashedWord = hash(ht, old); // This is the hash number by the key and salts.
        hashedWord %= (ht->l); 
	// This is the hash number that is limited by the range of the array by
	// the length of the array.				
        listNode **exactNode = &(ht->h[hashedWord]); // Same thing as in findHT.
        ht->h[hashedWord] = insertLL(exactNode, old, new); 
	// Takes that exact node and inserts it to the top of the linked list.
	// Frees both of the strings. 
	return;
}

void printHT(const hashTable *ht) // Prints out the hash table.
{
	uint32_t count = 0;
	for(uint32_t index = 0; index < (ht->l); index++)

	{
		if((ht->h[index]) != NULL) 
		// If the index of that array is not NULL, it prints the linked list. 
		{
			printLL(ht->h[index]); 
			++count;
		}
	}
	printf("%d", count);
	return;
}
