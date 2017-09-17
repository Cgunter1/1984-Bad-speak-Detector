// # DARRELL LONG ASSIGNMENT 3 hash function for bloomfilter.
# include "aes.h"
# include "bf.h"
# include <string.h>

static inline int realLength(int l)
{
        return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

uint32_t hashBF(bloomF *h, char *key) 
// Hash function for bloom filter that gives a function number back from the given string.
{
	uint32_t output[4] = { 0x0 };
        uint32_t sum       = 0x0;
        int keyL           = strlen(key);
        uint8_t *realKey   = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t));

        memcpy(realKey, key, keyL);

        for (int i = 0; i < realLength(keyL); i += 16)
        {
                AES128_ECB_encrypt((uint8_t *) h->s,            // Salt
                                   (uint8_t *) realKey + i,     // Input
                                   (uint8_t *) output);         // Output
                sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
        }
        free(realKey);
        return sum;
	
}
