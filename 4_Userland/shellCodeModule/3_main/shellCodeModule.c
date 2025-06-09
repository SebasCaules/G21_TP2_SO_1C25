// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
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
    return OK;
}
