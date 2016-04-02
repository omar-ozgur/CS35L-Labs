/* 
 * Name: Omar Ozgur
 * ID: 704465898
 * Date: 02/05/2016
 * Lab Section: 3
 * TA: Lauren Samy
 *
 * *** Assignment 5 ***
 *
 * */

//sfrob.c

#include <stdio.h>
#include <stdlib.h>

// Compare two frobnicated strings that end with spaces
// Return -1 if a < b, 0 if a = b, or 1 if a > b
int frobcmp(char const *a, char const *b) {
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
 	frobcmp(a, b);
}

int main(int argc, char *argv[]){
	char **words = NULL;
	int numWords = 0;

	char *word = NULL;
	int numLetters = 0;	

	char c = getchar();
	if(ferror(stdin)){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}

	char next = getchar();
	if(ferror(stdin)){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}

	while(c != EOF && !ferror(stdin)){

		// Add the current character to the current word
		numLetters++;
		char *longerWord = realloc(word, numLetters * sizeof(char));
		if(longerWord){
			word = longerWord;
			word[numLetters - 1] = c;
		}
		else{
			fprintf(stderr, "Memory allocation failed");
			exit(1);
		}
		
		// If the end of the word was reached, add the current word to the "words" array,
		// and prepare to create a new word
		if(c == ' '){
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
		if(c == ' ' && next == EOF){
			break;
		}
		if(c == ' ' && next == ' '){
			while(c == ' ') {
				c = getchar();
				
				// Check for errors while reading characters
				if(ferror(stdin)){
					fprintf(stderr, "Error while reading character");
					exit(1);
				}
			}
			next = getchar();
		}
		else if(next == EOF){
			c = ' ';
		}
		else{
			c = next;
			next = getchar();
		}
	}

	// Check if the "while" loop stopped due to a read error
	if(ferror(stdin)){
		fprintf(stderr, "Error while reading character");
		exit(1);
	}
	
	// Sort the frobnicated words
	qsort(words, numWords, sizeof(char **), cmp);
	
	// Output the sorted frobnicated words
	for(int i = 0; i < numWords; i++) {
		for(int j = 0; ; j++) {
			if(putchar(words[i][j]) == EOF){
				fprintf(stderr, "Error while writing character");
				exit(1);
			}
			if(words[i][j] == ' ')
				break;
		}
		free(words[i]);
	}
	free(words);

	return 0;
}
