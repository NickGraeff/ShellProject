#include "commands.h"
#include "customCommands.h"

static void 
shiftRemainingCharactersLeft (charVector *userInput) {
	
	/* Shift all characters left one */
	int i = userInput->cursorPosition + 1;
	while (i <= userInput->lastPlaceInString) {
		userInput->buffer[i-1] = userInput->buffer[i];
		putchar(userInput->buffer[i]);
		++i;
	}
	putchar(' ');

	/* Place cursor back at it's original position */
	i = userInput->cursorPosition;
	while (i < userInput->lastPlaceInString) {
		putchar('\b');
		++i;
	}
}

static void 
clearInput (charVector *userInput) {

	/* Place whitespace on the screen for the remainder of the string */
	while (userInput->cursorPosition < userInput->lastPlaceInString) {
		putchar(' ');	
		++userInput->cursorPosition;
	}

	/* Place whitespace everywhere before the end of the string and reset cursor position */
	while (userInput->cursorPosition) {
		putchar('\b');
		putchar(' ');
		putchar('\b');
		--userInput->cursorPosition;
	}

	userInput->cursorPosition = userInput->lastPlaceInString;
}

static void 
handleArrows (char arrowKey, charVector **userInput, vectorVector *historyUserInput) {

	switch (arrowKey) {

		/* User typed the up arrow key */
		case 'A':

			/* Check if user has already navigated to the top of the list */
			if (historyUserInput->userHistoryIterator != historyUserInput->leastRecentUserInput) {

				clearInput (*userInput);
				
				historyUserInput->userHistoryIterator = historyUserInput->userHistoryIterator->prevInput;
				*userInput = historyUserInput->userHistoryIterator;

				printf("%s", (*userInput)->buffer);
			}
			break;

		/* User typed the down arrow key */
		case 'B':

			/* Check if user has already navigated to the bottom of the list */
			if (historyUserInput->userHistoryIterator != historyUserInput->mostRecentUserInput) {

				clearInput (*userInput);
			
				historyUserInput->userHistoryIterator = historyUserInput->userHistoryIterator->nextInput;
				*userInput = historyUserInput->userHistoryIterator;

				printf("%s", (*userInput)->buffer);
			}
			break;

		/* User typed the right arrow key */
		case 'C':

			/* Check if user has already navigated to the end of the string */
			if ((*userInput)->cursorPosition != (*userInput)->lastPlaceInString) {
				putchar((*userInput)->buffer[(*userInput)->cursorPosition]);
				++(*userInput)->cursorPosition;
			}
			break;

		/* User typed the left arrow key */
		case 'D':

			/* Check if user has already navigated to the beginning of the string */
			if ((*userInput)->cursorPosition != 0) {
				putchar('\b');
				--(*userInput)->cursorPosition;
			}
			break;
	}

}

static void 
handleBackspaceandDelete (char userChar, charVector *userInput) {

	/* Check if user entered backspace */
	if (userChar == 127) {

		/* Check if the user has already navigated to the beginning of the string  */
		if (userInput->cursorPosition != 0) {
			
			/* Delete the character behind the cursor's position */
			--userInput->cursorPosition;
			userInput->buffer[userInput->cursorPosition] = ' ';
			putchar('\b');
			putchar(userInput->buffer[userInput->cursorPosition]);
			putchar('\b');

			shiftRemainingCharactersLeft (userInput);
			--userInput->lastPlaceInString;
		} 
	} 
	
	/* Otherwise the user entered delete */
	else {

		/* Check if the user has already navigated to the end of the string */
		if (userInput->cursorPosition != userInput->lastPlaceInString) {

			/* Delete the character where the cursor is */
			userInput->buffer[userInput->cursorPosition] = ' ';
			putchar(userInput->buffer[userInput->cursorPosition]);
			putchar('\b');

			shiftRemainingCharactersLeft (userInput);
			--userInput->lastPlaceInString;
		}
	}
}

void 
getAndDisplayUserInput (charVector **userInput, vectorVector *historyUserInput) {

	/* Display the user's current directory */
	{
		/* The userName and hostName allocations aren't precise, just copy and pastes of cwd */
		char *hostName = (char *) malloc(100);
		char *userName = getlogin();
		char *cwd = (char *) malloc(pathconf(".", _PC_PATH_MAX));
		getcwd(cwd, pathconf(".", _PC_PATH_MAX));
		gethostname(hostName, 100);
		printf("%s@%s:%s$ ", userName, hostName, cwd);
		free(cwd);
		free(hostName);
	}
	
	/* Disable regular IO for the shell */
	struct termios origConfig;
	tcgetattr(0, &origConfig);
	struct termios newConfig = origConfig;
	newConfig.c_lflag &= ~(ICANON|ECHO);
	newConfig.c_cc[VMIN] = 1;
	newConfig.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &newConfig);

	char userChar;
	while ((userChar = getchar()) != '\n') {

		/* The userInput buffer needs to be resized */
		if ((*userInput)->lastPlaceInString + 1 == (*userInput)->size) {
			(*userInput)->size *= 2;
			if (!((*userInput)->buffer = realloc((*userInput)->buffer, (*userInput)->size))) {
				printf("\nHad an error allocating memory for some reason. Sorry mate.\n");
				tcsetattr(0, TCSANOW, &origConfig);
				exit(1);
			}
		}

		/* The user entered an arrow key */
		if (userChar == '\033') {
			getchar();				//Disregard second character
			handleArrows(getchar(), userInput, historyUserInput);

		} 

		/* The user entered backspace or delete, for whatever reason tilde (126) also acts as delete */
		else if (userChar == 8  || userChar == 127 || userChar == 126) {
			handleBackspaceandDelete(userChar, *userInput);
		}

		/* Nothing special about the input, display and store the character and repeat */
		else {
			putchar(userChar);
			(*userInput)->buffer[(*userInput)->cursorPosition] = userChar;
			++(*userInput)->cursorPosition;
			++(*userInput)->lastPlaceInString;
		}
	}

	/* NULL terminate the user's input and set cursor position to end of string */
	(*userInput)->buffer[(*userInput)->lastPlaceInString+1] = '\0';
	(*userInput)->cursorPosition = (*userInput)->lastPlaceInString;

	/* Reenable regular IO for the shell */
	putchar('\n');
	tcsetattr(0, TCSANOW, &origConfig);
}

void 
parseInput (char *bufferCopy, executionMatrix *executionArray) {
	int commandRow = 0;
	int argRow = 1;
	char *spacesPointer;
	char *pipePointer;
	char *pipeAndSpacesPointer;
	
	/* We assume at first there are three commands max with one argument max each */
	executionArray->size = 3;
	executionArray->actualSize = 0;
	executionArray->commands = (commandMatrix *) calloc(executionArray->size, sizeof(commandMatrix));
	{
		int i = 0;
		while (i < executionArray->size) {
			executionArray->commands[i].size = 3;
			executionArray->commands[i].actualSize = 0;
			executionArray->commands[i].args = (char **) calloc(3, sizeof(char *));
			++i;
		}
	}

	/* 
	 * Parse the input by the spaces first and the pipes second 
	 * Note that I did not use strtok because I did not see anyway
	 * to use it with both symbols where the spaces were parsed
	 * before the pipes
	 */

	/* pipePointer points at the first character in the string */
	pipePointer = bufferCopy;				

	/* Iterate through the string until the end is reached */
	while (*pipePointer != '\0') {

		/* spacesPointer initially points at the first non-space character in the string or after a pipe */
		spacesPointer = pipePointer; 

		/* If it does initially point at a non-space character, set the first command to this spot */
		if (*spacesPointer != ' ') {
			executionArray->commands[commandRow].args[0] = spacesPointer;
		} 
		
		/* Otherwise, look for the first non-space character in the loop below */
		else {
			argRow = 0;
		}

		/* Search until the end of the string or a pipe is reached */
		while (*spacesPointer != '\0' && *spacesPointer != '|') {

			/* Check if the arguments array needs to be resized for the next argument */
			if (argRow == executionArray->commands[commandRow].size-1) {
				executionArray->commands[commandRow].size *= 2;
				if (!(executionArray->commands[commandRow].args = realloc(executionArray->commands[commandRow].args, executionArray->commands[commandRow].size * sizeof(char *)))) {
					printf("\nHad an error allocating memory for some reason. Sorry mate.\n");
					exit(1);
				}
			}

			/* If this character is the first non-space character after a space, set the next argument to this spot */
			if (*(spacesPointer-1) == ' ' && *spacesPointer != ' ') {
				executionArray->commands[commandRow].args[argRow] = spacesPointer;	
				++argRow;
			}

			/* Check the next character */
			++spacesPointer;
		}

		/* Set the last argument to NULL and set the number of arguments */
		executionArray->commands[commandRow].actualSize = argRow;
		executionArray->commands[commandRow].args[argRow] = NULL;

		/* Check if the commands array needs to be resized for the next command */
		if (commandRow == executionArray->size) {
			executionArray->size *= 2;
			if (!(executionArray->commands = realloc(executionArray->commands, executionArray->size * sizeof(commandMatrix)))) {
				printf("\nHad an error allocating memory for some reason. Sorry mate.\n");
				exit(1);
			}
		}

		/* Set the pipePointer to the pipe char or the NULL char position */
		pipePointer = spacesPointer;

		/* If the position just set is NULL, the loop will terminate, otherwise get the next char */
		if (*pipePointer != '\0') {
			++pipePointer;
		}
		
		/* Reset the argument row and increment the command row */
		argRow = 1;
		++commandRow;
	}

	/* Set the number of commands found */
	executionArray->actualSize = commandRow;

	/* Place NULL characters in all the spots where a space or pipe was found */
	pipeAndSpacesPointer = strtok(bufferCopy, " |");
	while (pipeAndSpacesPointer != NULL) {
		pipeAndSpacesPointer = strtok(NULL, " |");
	}
	
	/* Allocate memory for each new string */
	{
		int i;
		int j;
		for (i = 0; i < executionArray->actualSize; ++i) {
			for (j = 0; j < executionArray->commands[i].actualSize; ++j) {
				char *temp = executionArray->commands[i].args[j];
				executionArray->commands[i].args[j] = (char *) calloc(strlen(executionArray->commands[i].args[j]) + 1, sizeof(char));
				strcpy(executionArray->commands[i].args[j], temp);
			}
		}
	}
	
	/* If the userInput was empty, just point the first command at NULL */
	if (executionArray->actualSize == 0) {
		executionArray->commands[0].args[0] = (char *) calloc(1, sizeof(char));
		executionArray->commands[0].args[0][0] = '\0';
		executionArray->actualSize = 1;
		executionArray->commands[0].actualSize = 1;
	}
}

void 
runTheProcesses (executionMatrix *executionArray, vectorVector *historyUserInput) {
	int pid;
	int status;
	int fd[2];
	int inFile;
	int i;
	int numberOfPipes = executionArray->actualSize - 1;
	
	/* Go through each command and pipe its output to the next command if necessary */
	for (i = 0; i < executionArray->actualSize; ++i) {

		/* Only accounts for empty user input, doesn't help for bash-like scripting */
		if (checkForCustomCommands(executionArray, historyUserInput)) {
			continue;
		}

		pipe(fd);
		pid = fork();
		if (pid == 0) {
			if (numberOfPipes == 0) {
				close(fd[0]);
				close(fd[1]);
			} else if (i == 0) {
				close(1);
				dup(fd[1]);
				close(fd[0]);
			} else if (i < numberOfPipes) {
				close(0);
				dup(inFile);
				close(1);
				dup(fd[1]);
			} else {
				close(0);
				dup(inFile);
				close(fd[1]);
			}
			execvp(executionArray->commands[i].args[0], executionArray->commands[i].args);
			perror(executionArray->commands[i].args[0]);
		}
		waitpid(pid, &status, WUNTRACED);
		close(fd[1]);
		inFile = fd[0];
	} 
}
