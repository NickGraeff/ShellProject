#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>

typedef struct {
	char *buffer;
	int size;
	int actualSize;
	int lastPlaceInString;
	int place;
} charVector;

typedef struct {
	charVector *bufferOfBuffers;
	int size;
	int place;
	int placeInContext;
	int actualSize;
} vectorVector;

void getAndDisplayUserInput (charVector *userInput, vectorVector *historyUserInput);
void handleArrows (char arrowKey, charVector *userInput, vectorVector *historyUserInput);
void parseInput ();



int main () {
	/*Store the user's input in this vector struct */
	charVector userInput;
	userInput.size = 64;

	/* Store the user's input in this vector of vectors struct */
	vectorVector historyUserInput;
	historyUserInput.size = 2;
	historyUserInput.placeInContext = 0;
	historyUserInput.actualSize = 0;
	historyUserInput.bufferOfBuffers = (charVector *) calloc(historyUserInput.size, sizeof(charVector));

	/* Begin the program */
	int i = 0;
	for (; i < 5; ++i) {

		/* Reset buffer values */
		userInput.place = 0;
		userInput.actualSize = 0;
		userInput.lastPlaceInString = 0;
		userInput.buffer = (char *) calloc(userInput.size, sizeof(char));

		/* Get the user input */
		char *cwd = (char *) malloc(pathconf(".", _PC_PATH_MAX));
		getcwd(cwd, pathconf(".", _PC_PATH_MAX));
		printf("%s> ", cwd);
		getAndDisplayUserInput(&userInput, &historyUserInput);

		/* Place the user input into an array */

		/* Custom commands */

		/* Pass the input into the system  */

	}



	/* Free all the memory */

	return 0;
}

void getAndDisplayUserInput (charVector *userInput, vectorVector *historyUserInput) {
	/* Disable regular IO for the shell */
	struct termios origConfig;
	tcgetattr(0, &origConfig);
	struct termios newConfig = origConfig;
	newConfig.c_lflag &= ~(ICANON|ECHO);
	newConfig.c_cc[VMIN] = 10;
	newConfig.c_cc[VTIME] = 2;
	tcsetattr(0, TCSANOW, &newConfig);


	if (historyUserInput->actualSize == historyUserInput->size) {
		historyUserInput->size *= 2;
		if (!(historyUserInput->bufferOfBuffers = realloc(historyUserInput->bufferOfBuffers, historyUserInput->size * sizeof(charVector)))) {
			printf("\nHad an error allocating memory for some reason. Sorry mate.\n");
			tcsetattr(0, TCSANOW, &origConfig);
			exit(0);
		}
	}
	
	/* Store this new user input into the history  */
	{
		int i;
		for (i = historyUserInput->actualSize; i > 0; --i) {
			historyUserInput->bufferOfBuffers[i] = historyUserInput->bufferOfBuffers[i-1];
		}
	}
	++historyUserInput->actualSize;
	
	
	char userChar;
	while ((userChar = getchar()) != '\n') {

		/* The userInput buffer needs to be resized */
		if (userInput->lastPlaceInString + 1 == userInput->size) {
			userInput->size *= 2;
			if (!(userInput->buffer = realloc(userInput->buffer, userInput->size))) {
				printf("\nHad an error allocating memory for some reason. Sorry mate.\n");
				tcsetattr(0, TCSANOW, &origConfig);
				exit(0);
			}

			/* If user is at the bottom of the list, save their input */
			if (historyUserInput->placeInContext == 0) {
				historyUserInput->bufferOfBuffers[0] = *userInput;
			}
		}

		/* The user entered an arrow key */
		if (userChar == '\033') {
			getchar();				//Disregard second character
			handleArrows(getchar(), userInput, historyUserInput);

		} 

		/* The user entered backspace or delete */
		else if () {

		}

		/* Nothing special about the input, display and store the character and repeat */
		else {
			putchar(userChar);
			userInput->buffer[userInput->place] = userChar;
			++userInput->place;
			++userInput->lastPlaceInString;
		}
	}

	/* NULL terminate the user's input */
	userInput->buffer[userInput->place] = '\0';

	/* Reenable regular IO for the shell */
	putchar('\n');
	historyUserInput->placeInContext = 0;
	historyUserInput->bufferOfBuffers[0] = *userInput;
	tcsetattr(0, TCSANOW, &origConfig);
}

void handleArrows (char arrowKey, charVector *userInput, vectorVector *historyUserInput) {

	/* If user is at the bottom of the list, save their input */
	if (historyUserInput->placeInContext == 0) {
		historyUserInput->bufferOfBuffers[0] = *userInput;
	}

	switch (arrowKey) {

		/* User typed the up arrow key */
		case 'A':

			/* Check if user has already navigated to the top of the list */
			if (historyUserInput->placeInContext != historyUserInput->actualSize-1) {
				++historyUserInput->placeInContext;
				
				/* Place whitespace on the screen for the remainder of the string */
				while (userInput->place < userInput->lastPlaceInString) {
					putchar(' ');	
					++userInput->place;
				}

				/* Place whitespace everywhere before the end of the string and reset cursor position  */
				while (userInput->place) {
					putchar('\b');
					putchar(' ');
					putchar('\b');
					--userInput->place;
					--userInput->lastPlaceInString;
				}

				*userInput = historyUserInput->bufferOfBuffers[historyUserInput->placeInContext];

				printf("%s", userInput->buffer);
			}
			break;

		/* User typed the down arrow key */
		case 'B':

			/* Check if user has already navigated to the bottom of the list */
			if (historyUserInput->placeInContext != 1) {
				--historyUserInput->placeInContext;
				
				/* Place whitespace on the screen for the remainder of the string */
				while (userInput->place < userInput->lastPlaceInString) {
					putchar(' ');	
					++userInput->place;
				}

				/* Place whitespace everywhere before the end of the string and reset cursor position  */
				while (userInput->place) {
					putchar('\b');
					putchar(' ');
					putchar('\b');
					--userInput->place;
					--userInput->lastPlaceInString;
				}

				*userInput = historyUserInput->bufferOfBuffers[historyUserInput->placeInContext];

				printf("%s", userInput->buffer);
			}
			break;

		/* User typed the right arrow key */
		case 'C':

			/* Check if user has already navigated to the end of the string */
			if (userInput->place != userInput->lastPlaceInString) {
				putchar(userInput->buffer[userInput->place]);
				++userInput->place;
			}
			break;

		/* User typed the left arrow key */
		case 'D':

			/* Check if user has already navigated to the beginning of the string */
			if (userInput->place != 0) {
				putchar('\b');
				--userInput->place;
			}
			break;
	}

}

void parseInput () {

}

