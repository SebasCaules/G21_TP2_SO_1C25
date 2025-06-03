#include "shellFunctions.h"

int main() {
    initShell();
    
    int returned = OK;
    
    while(returned != EXIT) {
        prompt();
        char command[MAX_COMMAND_LENGTH + 1];
        gets(command, MAX_COMMAND_LENGTH);
        returned = getCmdInput(command);
    }
}
