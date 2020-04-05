#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "assignment7.h"

/** Below is Joe Van Boxtel's code
**/
void printStrings(char** strings, unsigned int count) {
	for (int i = 0; i < count; i++) {
			printf("%s\n", strings[i]);
	}
}

char** readWordsInFile(char *path, int *count) {
	int capacity = 10000;
	int localCount = 0;
	char ** words = malloc(sizeof(char *) * capacity);
	if (words == NULL) {
		printf("malloc of %d failed\n", capacity);
		exit(1);
	}

	FILE * file = fopen(path, "r");
	char buffer[1024] = {0};

	while (fgets(buffer, 1023, file) != NULL) {
		int start = 0;
		int index = 0;
		while (buffer[index] != '\0' && buffer[index] != '\n') {
			if (buffer[index] == ' ') {
				int wordLength = index - start;
				words[localCount] = malloc(sizeof(char) * (wordLength+1));
				strncpy(words[localCount], buffer+start, wordLength);
				words[localCount][wordLength] = '\0';

				localCount++;
				if (localCount >= capacity) {
					printf("Reallocing %d >= %d\n", localCount, capacity);
					capacity *= 2;
					words = reallocf(words, sizeof(char *) * capacity);
					if (words == NULL) {
						printf("Realloc of %d failed\n", capacity);
						exit(1);
					}
				}

				start = -1;
			} else if (start == -1) {
				start = index;
			}
			index++;
		}
	}

	*count = localCount;
	return words;
}

char** generateLines(int* count) {
	int localCount = 1000000;
	char **strings = malloc(localCount * sizeof(char*));
	for (int i = 0; i < localCount; i++) {
		char * word = malloc(7 * sizeof(char));
		int j;
		for (j = 0; j < 6; j++) {
			word[j] = ((i+1)*7 + (j+1)) % ('z'-'a') + 'a';
		}
		word[j] = '\0';
		strings[i] = word;
	}
	*count = localCount;
	return strings;
}

int main(void) {
	
	int count;
	char* path = "./textFiles/webster_random";
	char **strings = readWordsInFile(path, &count);
	//char **strings = generateLines(&count);
	
	clock_t start = clock();
	
	setSortThreads(10);
	sortThreaded(strings, count);
	
	clock_t end = clock();
	
	
	// printStrings(strings, count);
	
	printf("\nTook %lf seconds\n\n", ((double)(end-start))/CLOCKS_PER_SEC);
	return 0;
}
