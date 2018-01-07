#include <string.h>
#include <unistd.h>
#include "customCommands.h"

/* ISSUE: Needs to be entered multiple times if previous output was rejected */
static void 
handleQuit (executionMatrix *executionArray) {
	continueLoop = 0;
}

/* ISSUES: previousDirectory is not set on the first run, could result in error */
static void 
handleCD (executionMatrix *executionArray) {
	char *cwd = (char *) malloc(pathconf(".", _PC_PATH_MAX));
	getcwd(cwd, pathconf(".", _PC_PATH_MAX));
	static char *previousDirectory;

	if (executionArray->commands[0].args[1] == NULL) {
		return;
	}

	if (!strcmp(executionArray->commands[0].args[1], "$HOME")) {
		char *userName = getlogin();
		const int maxDirectorySize = 10;
		char *homeDirectory = (char *) calloc(maxDirectorySize, sizeof(char));
		strcpy(homeDirectory, "/home/");
		if (!(homeDirectory = realloc(homeDirectory, maxDirectorySize + strlen(userName)))) {
			perror("Problem allocating memory, sorry mate.");
			return;
		}
		strcat(homeDirectory, userName);

		if (chdir(homeDirectory)) {
			perror("Couldn't change to that directory sorry mate.");
		}

		free(homeDirectory);
	} else if (!strcmp(executionArray->commands[0].args[1], "-")) {
		if (chdir(previousDirectory)) {
			perror("Couldn't change to that directory sorry mate");
		}
	} else {
		if (chdir(executionArray->commands[0].args[1])) {
			perror("Couldn't change to that directory sorry mate.");
		}
	}

	free(previousDirectory);	
	previousDirectory = cwd;
}

/* ISSUE: Very little error checking, also displays to stdout AND file, should only be file */
static void
handleMerge (executionMatrix *executionArray) {
	int argNum = 1;

	free(executionArray->commands[0].args[0]);
	executionArray->commands[0].args[0] = (char *) calloc(4, sizeof(char));
	strcpy(executionArray->commands[0].args[0], "cat");

	while (strcmp(executionArray->commands[0].args[argNum], ">")) {
		++argNum;	
	}

	free(executionArray->commands[0].args[argNum]);
	executionArray->commands[0].args[argNum] = NULL;

	int pid = fork();
	int status;
	if (pid == 0) {
		close(1);
		dup((long)fopen(executionArray->commands[0].args[argNum+1], "w"));
		execvp(executionArray->commands[0].args[0], executionArray->commands[0].args);
		perror(executionArray->commands[0].args[0]);
		return;
	}
	waitpid(pid, &status, WUNTRACED);
}

/* Results are not left aligned */
static void 
handleHistory (executionMatrix *executionArray, vectorVector *historyUserInput) {
	int i;

	charVector *temp = historyUserInput->leastRecentUserInput;

	for (i = 0; i < historyUserInput->size; ++i) {
		printf(" %i  %s\n", i+1, temp->buffer);	
		temp = temp->nextInput;
	}
}

static void
handlePause (executionMatrix *executionArray) {
	struct termios origConfig;
	tcgetattr(0, &origConfig);
	struct termios newConfig = origConfig;
	newConfig.c_lflag &= ~(ICANON|ECHO);
	newConfig.c_cc[VMIN] = 1;
	newConfig.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &newConfig);

	while (getchar() != '\n');

	tcsetattr(0, TCSANOW, &origConfig);
}

/* ISSUE: This needs to use a hash in the future to save time */
int 
checkForCustomCommands (executionMatrix *executionArray, vectorVector *historyUserInput) {

	char *firstCommand = executionArray->commands[0].args[0];

	if (firstCommand == NULL) {
		//do nothing
	} else if (!strcmp(firstCommand, "quit")) {
		handleQuit (executionArray);
	} else if (!strcmp(firstCommand, "cd")) {
		handleCD (executionArray);
	} else if (!strcmp(firstCommand, "merge")) {
		handleMerge (executionArray);
		return 0;
	} else if (!strcmp(firstCommand, "history")) {
		handleHistory (executionArray, historyUserInput);
	} else if (!strcmp(firstCommand, "pause")) {
		handlePause (executionArray);
	} else {
		return 0;
	}

	return 1;
}
