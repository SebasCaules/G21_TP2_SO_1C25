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

#define WELCOME_MESSAGE "Welcome to ChabonOS, %s!\n"
#define PROMPT "%s@userland ~ $ "
#define NUM_MODULES 14

#define MAX_COMMAND_LENGTH 128
#define MAX_USERNAME_LENGTH 32

typedef struct module {
    char *name;
    void (*function)();
} module;


/**
 * @brief Displays a help message listing available commands.
 */
void help();

/**
 * @brief Shows the current system time on the terminal.
 */
void showTime();

/**
 * @brief Changes the font scale in the terminal.
 * @param scale The new font scale factor.
 */
void changeFontScale(int scale);

/**
 * @brief Clears the terminal screen.
 */
void clearTerminal();

/**
 * @brief Handles the spotify (clone) interface
*/
void spotifyInterface();

/**
 * @brief Handles the piano interface
*/
void pianoInterface();

/**
 * @brief Retrieves and displays the current values of CPU registers.
 */
void getRegs();

/**
 * @brief Prompts the user for input, used for the username.
 */
void askForUser();

/**
 * @brief Captures and processes user input for commands in the shell.
 */
void getCmdInput();

/**
 * @brief Initializes and starts the shell environment.
 */
void initShell();

