#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int continueLoop = 1; //Loop variable controls program duration
#include "commands.h"

int main () {

	/*Store the user's input in this vector struct */
	charVector *userInput = (charVector *) calloc(1, sizeof(charVector));
	userInput->size = 64;
	userInput->lastPlaceInString = 0;
	userInput->cursorPosition = 0;
	userInput->buffer = (char *) calloc(userInput->size, sizeof(char));
	userInput->nextInput = NULL;
	userInput->prevInput = NULL;

	/* Store the user's input in this vector of vectors struct */
	vectorVector historyUserInput;
	historyUserInput.size = 0;
	historyUserInput.mostRecentUserInput = userInput;
	historyUserInput.leastRecentUserInput = userInput;
	historyUserInput.userHistoryIterator = userInput;

	/* Begin the program */
	while (continueLoop) {

		/* Get the user input */
		getAndDisplayUserInput(&userInput, &historyUserInput);

		/* Place the user input into an array */
		char *bufferCopy = (char *) calloc(userInput->lastPlaceInString + 1, sizeof(char));
		strcpy(bufferCopy, userInput->buffer);

		executionMatrix executionArray;
		parseInput(bufferCopy, &executionArray);

		/* Pass the input into the system */
		runTheProcesses(&executionArray, &historyUserInput);

		/* Free the commands array and their string representations from memory */
		{
			int i;
			int j;
			for (i = 0; i < executionArray.size; ++i) {
				for (j = 0; j < executionArray.commands[i].actualSize; ++j) {
					free(executionArray.commands[i].args[j]);
				}
				free(executionArray.commands[i].args);
			}
			free(executionArray.commands);
			free(bufferCopy);
		}

		/* Create a new userInput */
		++historyUserInput.size;
		userInput->nextInput = (charVector *) calloc(1, sizeof(charVector));
		userInput = userInput->nextInput;
		userInput->nextInput = NULL;
		userInput->prevInput = historyUserInput.mostRecentUserInput;
		historyUserInput.mostRecentUserInput = userInput;
		historyUserInput.userHistoryIterator = userInput;
		userInput->size = 64;
		userInput->lastPlaceInString = 0;
		userInput->cursorPosition = 0;
		userInput->buffer = (char *) calloc(userInput->size, sizeof(char));
	}

	/* Free all the memory */

	return 0;
}
