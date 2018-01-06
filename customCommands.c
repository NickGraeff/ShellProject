#include <string.h>
#include "customCommands.h"

static void 
handleQuit (executionMatrix *executionArray) {
	exit(0);
}

static void 
handleCD (executionMatrix *executionArray) {
	if (!strcmp(executionArray->commands[0].args[1], "~")) {
		if (chdir("$HOME")) {
			perror("FU");
		}

	} else {
		if (chdir(executionArray->commands[0].args[1])) {
			perror("FU2");
		}
	}
}

static void
handleMerge () {

}

static void 
handleHistory () {

}

/* ISSUE: This needs to use a hash in the future to save time */
void 
checkForCustomCommands (executionMatrix *executionArray) {
	char *firstCommand = executionArray->commands[0].args[0];
	if (!strcmp(firstCommand, "quit")) {
		handleQuit (executionArray);
	} else if (!strcmp(firstCommand, "cd")) {
		handleCD (executionArray);
	} else if (!strcmp(firstCommand, "merge")) {
		handleMerge (executionArray);
	} else if (!strcmp(firstCommand, "history")) {
		handleHistory (executionArray);
	}
}
