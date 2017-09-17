# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <getopt.h>
# include <stdbool.h>
# include <ctype.h>
# include "bf.h"
# include "aes.h"
# include "ll.h"
# include "hash.h"

int yylex(void); 
// This sets up the yylex function to give out specific numbers depending
// on whether the number will be a 1 or -1. 

extern char* yytext;
// This variable collects the variable given by the call of yylex(). 

extern FILE* yyin;
// This accepts the place of text stream that yytext and yylex get their input.

uint32_t seeks;
// This collects the seeks that will be added to the output. These are found if they go through the linked list. 

double links;
// These collects all the words that have went through the hash table.

uint32_t wordLinks;

int main(int argc, char**argv)
{
	wordLinks = 0;
	FILE * fileBadspeak; // This sets a variable for the text stream from badspeak.txt.
	FILE * fileGoodspeak; // This sets a variable for the text stream from newspeak.txt.
	seeks = 0; // This automatically sets the global variable of seeks to 0.
	double average  = 0; // This sets the average to collect the average number of words that go through linked lists.
	int dictionary = 0; 
	// This dictionary sets up the dictionary that will hold all instances of words added into bloom filter.
	int translations = 0; // This holds all the translations that are present in the hashtable.
	int text = 0; // This counts all the words in the text given on the stdin.
	double density1 = 0; // This shows the density of the first bloom filter.
	double density2 = 0; // This shows the density of the second bloom filter.
	uint32_t badWordConversion = 0; // This checks if the text committed a thought crime.
	uint32_t badWords = 0; // Counts all badspeak words.
	uint32_t oldWordIndex = 0; 
	// Counts the number of oldspeak words within the arra that holds badspeak words to newspeak words.
	uint32_t newWordIndex = 0; 
	// Counts the number of newspeak words within the arrathat holds badspeak words to newspeak words.
	uint32_t thoughtCrime = 0; // Counts the number of thought crimes.
	uint32_t errorLength = 150; // Gives a length for the errorWords array.
	uint32_t errorCount = 0; // Counts the index for the errorWords.
	int numberOfArgs; // Number of args passed in the program.
	int messageNumber = 0; // Determines which message to print to the user.
	int programArugments; // This is used to determine what argument is given by the user.
	int token; // This variable is used to hold the output given by yylex().
	uint32_t hashTableLength = 10000; // This is the default hashtable length.
	uint32_t bFLength = 0x100000; // This is the default bloom filter length.
	uint8_t stats = 0; // This determines whether stats is called, which would equal 1.
	char oldBuffer[200]; // This gives a variable to the old word variable that will be put into the bloom filters.
	char newBuffer[200]; // This gives a variable to the new word variable that will be put into the hash table.
	char **oldWords; // This array holds old words that have a translation in new words found in the text.
	char **newWords; // This array holds new words that have a translation in old words found in the text.
	char **errorWords = (char **) malloc(sizeof(char *) * errorLength); // This holds an array for all error words.
	uint32_t bF1[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First BF Salts.
	uint32_t bF2[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second BF Salts.
	uint32_t hT[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // HashTable Salts.
	
	while((programArugments = getopt(argc, argv,"sh:f:")) != -1) // This goes through all the arguments given.
	{
		++numberOfArgs;
		switch(programArugments)
		{
			case 's': // Display stats.
			{
				++stats;
				break;
			}
			case 'h': // Change size of hashtable.
			{
				hashTableLength = atol(optarg);
				break;
			}
			case 'f': // Change size of bloom filters.
			{
				bFLength = atol(optarg);
				break;
			}
			default: // Nothing.
			{
				break;
			}
		}
	}
	// Initiates both BloomFilters and only hashTable.
	bloomF *bloomFilter1 = newBF(bFLength, bF1);
	bloomF *bloomFilter2 = newBF(bFLength, bF2);
	hashTable *hashT = newHT(hashTableLength, hT);
	// This opens the file of badspeak in darrell's directory for badspeak words.
	// And sets them to both bloom filter with counting them in the dictionary.
	fileBadspeak = fopen("badspeak.txt", "r");
	while(EOF != fscanf(fileBadspeak, "%s\n", oldBuffer))
	{
		setBF(bloomFilter1, oldBuffer);
		setBF(bloomFilter2, oldBuffer);
		insertHT(hashT, oldBuffer, "BAD");
		++dictionary;
	}
	// Closes the file.
	fclose(fileBadspeak);
	fileBadspeak = NULL;
	// Opens the file for newspeak and outputs the first word of every line to the oldBuffer
	// and the second word of every line to the newBuffer. Then sets every two words per line
	// to be inserted in the hashTable with insertHT.
	fileGoodspeak = fopen("newspeak.txt", "r");
	while(EOF != fscanf(fileGoodspeak, "%s %s\n", oldBuffer, newBuffer))
	{
		++translations;
		setBF(bloomFilter1, oldBuffer);
                setBF(bloomFilter2, oldBuffer);
		insertHT(hashT, oldBuffer, newBuffer);
		++badWords;
	}	
	fclose(fileGoodspeak);

        


	// Sets two arrays for oldWords and newWords.
	oldWords = (char **)malloc(sizeof(char *) * badWords);
        newWords = (char **)malloc(sizeof(char *) * badWords);
	// Sets the the text stream of yyin to stdin.
	yyin = stdin;
	// Sets token to 0;
	token = 0;
	// Sets error count to 0.	
	errorCount = 0;
	links = 0;
	
	while(token != -1) // Goes through the whole text stream until it reaches the end of the file.
	{
		token = yylex(); // Sets the token to equal the output of yylex() which is either 0 or -1.
		if(token == -1) // if token is -1, The program quits the while loop.
		{
			break;
		}
		++text;
		// The current word is the one that is given by yytext and is used to represent the currentWord being 
		// looked for by the yytext().
		char *currentWord = malloc(sizeof(char) * 1000); 
		strcpy(currentWord, yytext);
		int lengthOfCurrentWord = (strlen(currentWord))+1;
		// This lowercased all the characters for the variable currentWord.
		for(int index = 0; index < lengthOfCurrentWord; index++)
		{
			currentWord[index] = tolower(currentWord[index]);
		}
		// Checks if the current word is a 1 in the first bloom filter.	
		if(memBF(bloomFilter1, currentWord) == 1) // checks if word passes first filter.
		{
			// Checks if the current word is a 1 in the second bloom filter. 
			if(memBF(bloomFilter2, currentWord) == 1)
			{
				// This looks for the node in the hashTable given.
				listNode *tempHT = findHT(hashT, currentWord);
				// If it is NULL that shows that the word does not exist on the hashTable.
				if(tempHT != NULL)
				{
					if(strcmp(tempHT->newspeak,"BAD") != 0)
					{
						// Makes sure that the word was not already found as a 
						// good-speak to old-speak word.
						uint8_t copiedWord = 0;
						for(uint32_t index = 0; index < oldWordIndex; index++)
						{
							if(strcmp(oldWords[index], tempHT->oldspeak) == 0)
							{
								++copiedWord;
							}
						}
						// If not a copied word, the oldspeak and newspeak words are put 
						// in the oldWords array and newWords array respectively.
						if(copiedWord == 0)
						{
							oldWords[oldWordIndex] = tempHT->oldspeak;
							newWords[newWordIndex] = tempHT->newspeak;
							++oldWordIndex;
							++newWordIndex;
							++badWordConversion;
						}
				
					}
					else
					{
						// If it is not found on the HashTable, then the offense is constituted as a Thought Crime!!
						uint8_t criminalCopiedWord = 0;
						++thoughtCrime;
						// This increases the size of the array that will hold all the numbers that are constituted as thought crimes.
						if(errorLength == errorCount)
						{
							errorLength *= 2;
							errorWords = realloc(errorWords, sizeof(char *) * errorLength);
						}
						// Checks that there are duplicate words in the errorWords Array.
						for(uint32_t i = 0; i < errorCount; i++)
						{
							// If there is a duplicate, it adds nothing to the loop.
							if(strcmp(currentWord, errorWords[i]) == 0)
							{
								++criminalCopiedWord;
							}
						}
						// If the copied is there then pass. 
						if(criminalCopiedWord > 0)
						{
							//Do Nothing
						}
						// Then the word is original. Then add it to the array.
						else
						{
							errorWords[errorCount] = malloc(sizeof(char) * 1000);
							strcpy(errorWords[errorCount], currentWord);
							++errorCount;	
						}
					}
				}
			}
		}
		free(currentWord);
	}
	// Determines which message to give the user, either the 
	// first message for committing a thought crime, or the second one for not.
	if((badWordConversion > 0) || (thoughtCrime > 0))
	{
		if(thoughtCrime > 0)
		{
			++messageNumber;
		}
		else
		{
			messageNumber += 2;
		}
	}
	// Checks whether to print stats or not.
	if(stats <= 0)
	{
		if(messageNumber == 1)
		{
			printf("Dear Comrade,\n");
			printf("\nYou have chosen to use degenerate words that may cause hurt");
			printf("\nfeelings or cause your comrades to think unpleasant thoughts.");
			printf("\nThis is doubleplus bad. To correct your wrongthink and");
			printf("\nsave community consensus we will be sending you to joycamp");
			printf("\nadministered by Miniluv.\n");
			printf("\nYour errors:\n");
			// Prints out all the errors.
			for(int i = (int )errorCount - 1; i >= 0; i--)
			{
				printf("\n%s", errorWords[i]);
			}
			
		}
		else if(messageNumber == 2)
		{
			printf("Dear Comrade,\n");
			printf("\nSubmitting your text helps to preserve feelings and prevent");
			printf("\nbadthink. Some of the words that you used are not goodspeak.");
			printf("\nThe list shows how to turn the oldspeak words into newspeak.\n");
		}
		if(messageNumber == 1)
		{
			printf("\n\nThink on these words during your vacation!\n");
		}
		// Prints out all translations.
		for(int index = (int )oldWordIndex - 1; index >= 0; index--)
		{
			printf("\n%s -> %s", oldWords[index], newWords[index]);
		}
	}
	else
	{
		// Calculates all the 1-bits of both bloom filters.
		uint32_t sumOfBitsBF1 = 0;
		uint32_t sumOfBitsBF2 = 0;
		for(uint32_t x = 0; x <= bFLength; x++)
		{
			// Adds all of them up.
			sumOfBitsBF1 += valBF(bloomFilter1,x);
			sumOfBitsBF2 += valBF(bloomFilter2,x);
		}
		// Finds the average of times that the number goes through the loop.
		average = (double) wordLinks/(double )seeks;
		// Finds the densities of the bloom filters.
		density1 = (double ) sumOfBitsBF1 / bFLength;
		density2 = (double ) sumOfBitsBF2 / bFLength;	
		// Prints all the stats for the program.
		printf("Seeks %d, Average %lf, Dictionary %d, Translations %d, Text %d, Densities: %f, %f", 
			seeks, average, dictionary, translations, text, density1, density2);
	}
	printf("\n");
	// Deletes all the bloomfilters, newWords, oldWords, errorWords, hashTable.
	delBF(bloomFilter1);
        delBF(bloomFilter2);
	free(newWords);
	newWords = NULL;
        free(oldWords);
	oldWords = NULL;
	for(uint32_t index = 0; index < errorCount; index++)
	{
		free(errorWords[index]);
	}
	free(errorWords);
	errorWords = NULL;
	delHT(hashT);	
	return 0;
}
