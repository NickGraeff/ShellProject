#include "commands.h"

#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

extern int continueLoop;

/*
 * Checks if the user entered any custom commands,
 * if they did they will be executed. 
*/
int checkForCustomCommands (executionMatrix *executionArray, vectorVector *historyUserInput);

#endif /* CUSTOM_COMMANDS_H */
