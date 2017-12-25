#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "commands.h"

int main () {
	
	int continueLoop = 1; //Loop variable controls program duration

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
	while (continueLoop) {

		/* Reset buffer values */
		userInput.place = 0;
		userInput.actualSize = 0;
		userInput.lastPlaceInString = 0;
		userInput.buffer = (char *) calloc(userInput.size, sizeof(char));

		/* Get the user input */
		getAndDisplayUserInput(&userInput, &historyUserInput);

		/* Place the user input into an array */
		char *bufferCopy = (char *) calloc(userInput.lastPlaceInString+1, sizeof(char));
		strcpy(bufferCopy, userInput.buffer);

		executionMatrix executionArray;
		parseInput(bufferCopy, &executionArray);

		/* Custom commands */
		//Bad implementation of quit
		if (!strcmp(executionArray.commands[0].args[0], "quit")) {
			printf("Exiting the program...\n");
			break;
		}

		/* Pass the input into the system */
		runTheProcesses(&executionArray);

		/* Free the commands array from memory */
		{
			int j;
			for (j = 0; j < executionArray.size; ++j) {
				free(executionArray.commands[j].args);
			}
			free(executionArray.commands);
			free(bufferCopy);
		}
	}

	/* Free all the memory */

	return 0;
}
