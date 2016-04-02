/* 
 * Name: Omar Ozgur
 * ID: 704465898
 * Date: 02/22/2016
 * Lab Section: 3
 * TA: Lauren Samy
 *
 * *** Assignment 7 ***
 *
 * */

//sfrobu.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

long comparisons = 0;

// Compare two frobnicated strings that end with spaces
// Return -1 if a < b, 0 if a = b, or 1 if a > b
int frobcmp(char const *a, char const *b) {
	comparisons++;
	while(1){
		if((*a == ' ') && (*b == ' '))
			return 0;
		else if(*a == ' ')
			return -1;
		else if(*b == ' ')
			return 1;
		else if((*a ^ 42) < (*b ^ 42))
			return -1;
		else if((*a ^ 42) > (*b ^ 42))
			return 1;
		a++;
		b++;
	}
}

// Helper function to call frobcmp
int cmp(const void* a1, const void* a2){
 	const char* a = *(const char**)a1;
 	const char* b = *(const char**)a2;
 	return frobcmp(a, b);
}

int main(int argc, char *argv[]){
	struct stat fileStat;
	if(fstat(0,&fileStat) < 0){
		fprintf(stderr, "There was an error while reading from standard input");
		exit(1);
	}

	char **words = NULL;
	long numWords = 0;
	char *fileString;
	if(S_ISREG(fileStat.st_mode)){
		fileString = (char *)malloc(fileStat.st_size * sizeof(char));
		ssize_t fileRead = read(0, fileString, fileStat.st_size);
		if(fileRead < 0){
			fprintf(stderr, "There was an error while reading the file");
			exit(1);
		}
		int traverse = 0;
		for(long i = 0; i < fileStat.st_size; i++){
			if(i == fileStat.st_size - 1){
				fileString[i] = ' ';
			}
			if(traverse == 0 && fileString[i] != ' '){
				numWords++;
				traverse = 1;
			}
			else if(traverse == 1 && fileString[i] == ' '){
				traverse = 0;
			}
		}
		
		traverse = 0;
		long wordCounter = 0;
		words = (char **)malloc(numWords * sizeof(char *));
		for(long i = 0; i < fileStat.st_size; i++){
			if(traverse == 0 && fileString[i] != ' '){
				words[wordCounter] = &fileString[i];
				wordCounter++;
				traverse = 1;
			}
			else if(traverse == 1 && fileString[i] == ' '){
				traverse = 0;
			}	
		}
	}
	
	char *word = NULL;
	long numLetters = 0;

	char c[1];
	ssize_t cResult = read(0, c, 1);
	if(cResult < 0){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}
	
	char next[1];
	ssize_t nextResult = read(0, next, 1);
	if(nextResult < 0){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}
	
	while(cResult != 0){
		
		// Add the current character to the current word
		numLetters++;
		char *longerWord = realloc(word, numLetters * sizeof(char));
		if(longerWord){
			word = longerWord;
			word[numLetters - 1] = c[0];
		}
		else{
			fprintf(stderr, "Memory allocation failed");
			exit(1);
		}
		
		// If the end of the word was reached, add the current word to the "words" array,
		// and prepare to create a new word
		if(c[0] == ' '){
			numWords++;
			char **moreWords = realloc(words, numWords * sizeof(char *));
			if(moreWords){
				words = moreWords;
				words[numWords - 1] = word;
				
				word = NULL;
				numLetters = 0;
			}
			else{
				fprintf(stderr, "Memory allocation failed");
				exit(1);
			}
		}
		
		// Logic to append a space if there isn't one at the end of a file
		if(c[0] == ' ' && nextResult == 0){
			break;
		}
		if(c[0] == ' ' && next[0] == ' '){
			while(c[0] == ' ') {
				cResult = read(0, c, 1);
				
				// Check for errors while reading characters
				if(cResult < 0){
					fprintf(stderr, "Error while reading character");
					exit(1);
				}
			}
			nextResult = read(0, next, 1);
		}
		else if(nextResult == 0){
			c[0] = ' ';
		}
		else{
			c[0] = next[0];
			cResult = nextResult;
			nextResult = read(0, next, 1);
		}
	}

	// Check if the "while" loop stopped due to a read error
	if(cResult < 0 || nextResult < 0){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}
	
	// Sort the frobnicated words
	qsort(words, numWords, sizeof(char *), cmp);
	
	// Output the sorted frobnicated words
	for(int i = 0; i < numWords; i++) {
		long wordSize = 0;
		for(int j = 0; ; j++) {
			wordSize++;
			if(words[i][j] == ' ')
				break;
		}
		if(write(1, words[i], wordSize) < 0){
			fprintf(stderr, "Error while writing character");
			exit(1);
		}
	}
	free(words);

	fprintf(stderr, "Comparisons: %ld\n", comparisons);

	return 0;
}
