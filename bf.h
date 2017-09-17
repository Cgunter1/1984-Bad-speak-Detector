// CITATION: DARRELL LONG PIAZZA: @800

# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef _BF_H
# define _BF_H
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "aes.h"

typedef struct bloomF 
{
	uint8_t *v; // Vector
	uint32_t l; // Length
	uint32_t s[4]; // Salt;
} bloomF;

uint32_t hashBF(bloomF *b, char *key); // Gives the hash function number back from the bloomfilter's salt and string key.

static inline bloomF *newBF(uint32_t l, uint32_t b[]) // Creates a new bloomfilter for that bloom filter.
{
	bloomF *bloomFilter;
	bloomFilter = (bloomF *)malloc(sizeof(bloomF)); // Sets aside space for the bloomfilter.
	uint32_t length = l;
	bloomFilter->l = length;
	bloomFilter->v = (uint8_t *)calloc((l/8)+1, sizeof(uint8_t)); // Sets aside space for the bit vector.
	for(uint8_t x = 0; x < 4; x++)
	{
		bloomFilter->s[x] = b[x]; // Creates an array of salts.
	}
	return bloomFilter; // Returns bloomfilter.
}

static inline void delBF(bloomF *v) // Deletes the bloomfilter.
{
	free(v->v);
	v->v = NULL;
	free(v);
	return;
}

static inline uint32_t valBF(bloomF *x, uint32_t k) // Finds the value of the specified bit for the vector in question.
{
	uint32_t tableIndex = k / 8;
	uint32_t index = k % 8;
	return(x->v[tableIndex] >> index)%2; // Returns the either one or zero.
}

// Number of bits in Bloom Filter.
static inline uint32_t lenBF(bloomF *x) { return x->l; }

// Counts the on-bits. Gives back all the necessary bits for the vector.
static inline uint32_t countBF(bloomF *b)
{
	return b->l;
}

// Sets a bit for that vector that specified number given by the hashIndex.
static inline void setBF(bloomF *x, char *key)
{
	uint32_t hashIndex = hashBF(x, key);
	hashIndex %= lenBF(x);
	uint8_t orProperty = 1;
	uint32_t tableIndex = hashIndex / 8;
	uint32_t index = hashIndex % 8;
	orProperty = orProperty << index;
	x->v[tableIndex] |= orProperty;
	return;	
}

// Clears the bit for that vector that specified number given by the hashIndex.
static inline void clrBF(bloomF *x, char *key)
{
	uint32_t hashIndex = hashBF(x, key);
        hashIndex %= lenBF(x);
	uint8_t andProperty;
	uint32_t tableIndex = hashIndex / 8;
        uint32_t index = hashIndex % 8;
	andProperty = (uint8_t)(255 - pow(2, index));
	x->v[tableIndex] &= andProperty;
	return;
}
// Finds if the key given is a one on the specific bloomFilter.
static inline uint32_t memBF(bloomF *x, char *key)
{
	// Return 1 if found or 0 if not.
	uint32_t hashIndex = hashBF(x, key);
	hashIndex %= lenBF(x);
	return valBF(x, hashIndex);

}

// Prints out the number of the index where the bloomfilter is 1.
static inline void printBF(bloomF *x)
{
	for(uint32_t k = 0; k < (lenBF(x)); k++)
	{
		if(valBF(x, k) == 1)
		{
			printf("%u", k);
		}
	}
	return;
}

# endif
