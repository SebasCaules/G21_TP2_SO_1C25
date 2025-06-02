#include <stdint.h>
#include <stdLib.h>
#include <snake.h>
#include <exceptions.h>
#include <stddef.h>
#include <spotify.h>
#include <piano.h>

// Includes TESTS
#include <testMM.h>
#include <testProcess.h>
#include <testPriority.h>
#include <testSynchro.h>

#define WELCOME_MESSAGE "Welcome to ChabonOS, %s!\n"
#define PROMPT "%s@userland ~ "

#define ERROR -1
#define OK 0
#define EXIT 1

#define MAX_COMMAND_LENGTH 128
#define MAX_USERNAME_LENGTH 32
#define MAX_COMMANDS 2
#define MAX_ARGS 5

typedef struct command {
	char *name;
	uint8_t builtin;
	EntryPoint function;
} module;

typedef struct executable_command {
	char *command;
	char *args[MAX_ARGS + 1];
	int argc;
	int fds[2];
	int pid;
} executable_command_t;


/**
 * @brief Displays a help message listing available commands.
 */
int help();

/**
 * @brief Shows the current system time on the terminal.
 */
int showTime();

/**
 * @brief Changes the font scale in the terminal.
 * @param scale The new font scale factor.
 */
int changeFontScale(int argc, char *args[]);

/**
 * @brief Clears the terminal screen.
 */
int clearTerminal();

/**
 * @brief Handles the spotify (clone) interface
*/
int spotifyInterface();

/**
 * @brief Handles the piano interface
*/
int pianoInterface();

/**
 * @brief Retrieves and displays the current values of CPU registers.
 */
int getRegs();

/**
 * @brief Prompts the user for input, used for the username.
 */
void askForUser();

/**
 * @brief Captures and processes user input for commands in the shell.
 */
int getCmdInput(char* command);

/**
 * @brief Initializes and starts the shell environment.
 */
void initShell();

void prompt();
