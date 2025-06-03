#include <shellFunctions.h>
#include <stdLib.h>

typedef enum {
    BLUE = 4,
    YELLOW,
    CYAN,
    MAGENTA,
    GRAY,
    ORANGE,
    PURPLE,
    BROWN,
    PINK,
    LIME,
    NAVY,
    TEAL,
    OLIVE,
    MAROON,
    SILVER,
    GOLD,
    FDS_COUNT // Total de colores
} fds;

static void toUtcMinus3(time_struct * time);

static char username[MAX_USERNAME_LENGTH] = { 0 };

int help();
int showTime();
int clearTerminal();
int getRegs();
int song_player();
int isValidBase(const char *base);
int isNumberInBase(const char *num, const char *base);
int isConvertValid(char words[MAX_WORDS][MAX_WORD_LENGTH]);
int colorShowcase();

#define NUM_MODULES 17

static uint8_t foreground = 1;

int printA(int argc, char *argv[]) {
    while (1) {
        putchar('A');
        putchar('\n');
    }
    return 0;
}

static module modules[] = {
    {"help", 1, &help},
    {"time", 1, &showTime},
    {"setfont", 1, &changeFontScale},
    {"clear", 1, &clearTerminal},
    {"convert", 1, &convert},
    {"snake", 0, (EntryPoint) &snake},
    {"spotify", 0, (EntryPoint) &spotifyInterface},
    {"piano", 0, (EntryPoint) &pianoInterface},
    {"getregs", 1, &getRegs},
    {"opcode", 1, &opCode},
    {"divzero", 1, &divZero},
    {"testmm", 0, (EntryPoint) &testMM},
    {"ts", 0, (EntryPoint) &testProcess},
    {"tp", 0, (EntryPoint) &testPriority},
    {"tsem", 0, (EntryPoint)&test_sync},
    {"colorshow", 1, &colorShowcase},
    {"a", 0, (EntryPoint) &printA}
};


int help() {
    puts("Available Commands: ");
    puts("  help            - Shows all available commands.");
    puts("  time            - Display the current system time.");
    puts("  setfont <scale> - Adjust the font size. (1 or 2)");
    puts("  clear           - Clear the terminal screen.");
    puts("  convert         - <base_1> <base_2> <number>");
    puts("  snake           - Starts the Snake game.");
    puts("  spotify         - Starts the spotify (clone) interface.");
    puts("  piano           - Starts the piano interface.");
    puts("  getregs         - Display the registers's contents.");
    puts("  divzero         - Throws division by zero exc.");
    puts("  opcode          - Throws invalid opcode exc.");
    puts("  testmm          - Test the memory manager.");
    puts("  ts <max_proc>   - Test the scheduler manager.");
    puts("  tp              - Test the priority manager.");
    puts("  tsem            - Test the semaphore manager.");
    puts("  colorshow       - Show the color showcase.");
    return OK;
}

int showTime() {
    time_struct time;
    sys_time(&time);
    toUtcMinus3(&time);
    printf("[dd/mm/yyyy] - [hh:mm:ss]\n");
    printf(" %d/%d/20%d  -  %d:%d:%d  \n", time.day, time.month, 
    time.year, time.hour, time.minutes, time.seconds); //Porque year es solo 2 digs?
    return OK;
}

// int scale
int changeFontScale(int argc, char *args[]) {
    if (argc != 1) {
        fdprintf(STDERR, "Invalid command. (setfont <scale>) (1 or 2)\n");
        return ERROR;
    }
    int scale = satoi(args[0], NULL);
    if (setFontScale(scale) == -1) {
        fdprintf(STDERR, "Invalid command. (setfont <scale>) (1 or 2)\n");
        return ERROR;
    }
    return OK;
}

int clearTerminal() {
    clearView();
    return OK;
}

int spotifyInterface() {
	clearView();
    puts("Welcome to the Spotify (clone) interface!\n");
    puts("Here is a list of our repertoire:\n");
    puts("1 - The scale of C4.\n");
    puts("2 - The happy birthday song.\n");
    puts("3 - Adios nonino, Astor Piazzola.\n");
    puts("4 - Come together, The Beatles.\n");
    puts("Press q to go back to the terminal.\n");
    char c[MAX_COMMAND_LENGTH + 1];
    int readLength;
    do {
        putsNoNewLine("Choose the song you want to play: ");
        readLength = gets(c, MAX_COMMAND_LENGTH);
        if(readLength > 1) {
            puts("Input only one digit");
        } else if(c[0] >= '1' && c[0] <= (NUMBER_OF_SONGS+48)) {
            playSong(c[0]);
        } else {
            puts("Input a number between 1 and 4");
        }
    } while (c[0] != 'q');
    clearView();
    return OK;
}

int pianoInterface() {
    clearView();
    puts("Welcome to the piano simulator interface!\n");
    puts("The piano starts at the middle C octave, C4. Play the keys while holding SHIFT to increase the octave by one.\n");
    puts("\'W\':C, \'E\':D, \'R\':E, \'T\':F, \'Y\':G, \'U\':A, \'I\':B, \'O\':C\n");
    puts("\'3\':C#, \'4\':D#, \'6\':F#, \'7\':G#, \'8\':A#\n");
    // playKeys();
    clearView();
    return OK;
}

int getRegs() {
    uint64_t r[17];
    if(sys_get_regs(r)) {
        puts("Register snapshot:");
        printf("rax: %x\n", r[0]);
        printf("rbx: %x\n", r[1]);
        printf("rcx: %x\n", r[2]);
        printf("rdx: %x\n", r[3]);
        printf("rsi: %x\n", r[4]);
        printf("rdi: %x\n", r[5]);
        printf("rbp: %x\n", r[6]);
        printf("r8:  %x\n", r[7]);
        printf("r9:  %x\n", r[8]);
        printf("r10: %x\n", r[9]);
        printf("r11: %x\n", r[10]);
        printf("r12: %x\n", r[11]);
        printf("r13: %x\n", r[12]);
        printf("r14: %x\n", r[13]);
        printf("r15: %x\n", r[14]);
        printf("rsp: %x\n", r[15]);
        printf("rip: %x\n", r[16]);
    } else {
        fdprintf(STDERR, "There is no snapshot available.\n");
        return ERROR;
    }
    return OK;
}

void askForUser() {
    putsNoNewLine("Enter a username: ");
    gets(username, MAX_USERNAME_LENGTH);
}

void prompt() {
    printf(PROMPT, username);
    fdprintf(GOLD, "$");
}

static int fillCommandAndArgs(char **command, char *args[], char *input) {
	int argsCount = 0;
	foreground = 1;
	char *current = input;

	while (*current == ' ') {
		current++;
	}

	*command = current;

	while (*current != 0) {
		if (*current == '&' && (*(current + 1) == '\0' || *(current + 1) == ' ')) {
			foreground = 0;
			*current = 0;
			break;
		}

		if (*current == ' ') {
			*current = 0;

			if (*(current + 1) != 0 && *(current + 1) != ' ' &&
				*(current + 1) != '&') {
				args[argsCount++] = current + 1;
				if (argsCount >= MAX_ARGS) {
					break;
				}
			}
		}
		current++;
	}

	args[argsCount] = NULL;
	return argsCount;
}

int getCmdInput(char* command) {
    if(strlen(command) == 0){
        return;
    }
    
    executable_command_t executable_commands[MAX_COMMANDS];
	for (int i = 0; i < MAX_COMMANDS; i++) {
		executable_commands[i].pid = -1;
	}
    
    char *pipe_pos = strchr(command, '|');
	size_t executable_command_count = pipe_pos == NULL ? 1 : 2;
	if (pipe_pos) {
        *pipe_pos = 0;
		if (strchr(pipe_pos + 1, '|')) {
            fdprintf(STDERR, "Error: Only one pipe is allowed.\n");
			return ERROR;
		}
	};
    
    for (int i = 0; i < executable_command_count; i++) {
        executable_commands[i].argc = fillCommandAndArgs(
            &executable_commands[i].command, executable_commands[i].args, command);
        if (executable_commands[i].argc == ERROR) {
            return ERROR;
        }
        // if (pipe_pos)
        // 	command = pipe_pos + 1;
    }

    if (pipe_pos) {
		int pipefds[2];
		if (createPipe(pipefds) == -1) {
            fdprintf(STDERR, "Error creating pipe.\n");
			return ERROR;
		}
		executable_commands[0].fds[1] = pipefds[1];
		executable_commands[1].fds[0] = pipefds[0];
		executable_commands[1].fds[1] = STDOUT;
		executable_commands[0].fds[0] = STDIN;
	}
	else {
		executable_commands[0].fds[0] = foreground ? STDIN : -1;
		executable_commands[0].fds[1] = STDOUT;
	}

    for (int i = 0; i < executable_command_count; i++) {
        uint8_t found = 0;
        for (int j = 0; j < NUM_MODULES; j++) {
            if (strcmp(executable_commands[i].command, modules[j].name) == 0) {
                found = 1;
                if (modules[j].builtin) {
                    printf("Executing builtin command: %s\n", executable_commands[i].command);
                    printf("Arguments: ");
                    for (int k = 0; k < executable_commands[i].argc; k++) {
                        printf("%s ", executable_commands[i].args[k]);
                    }
                    putchar('\n');
                    return modules[j].function(executable_commands[i].argc, executable_commands[i].args);
                } else {
                    for (int a = 0; a < executable_commands[i].argc; a++) {
                        printf("arg[%d]: %s\n", a, executable_commands[i].args[a]);
                    }
                    executable_commands[i].pid = newProcess(
                        modules[j].function, executable_commands[i].args,
                        executable_commands[i].command, 0, executable_commands[i].fds);
                    printf("Executing external command: %s\n", executable_commands[i].command);
                    printf("pid: %d\n", executable_commands[i].pid);
                    yield();
                    
                    if (executable_commands[i].pid == -1) {
                        fdprintf(STDERR, "Error creating process for command: %s\n",  executable_commands[i].command);
                        return ERROR;
                    }
                    printf("pid: %d\n", executable_commands[i].pid);
                    
                }
                break;
            }
        }
        if (!found) {
            fdprintf(STDERR, "Command not found: %s\n", executable_commands[i].command);
            for (int j = 0; j < executable_command_count; j++) {
                if (executable_commands[j].pid != -1) {
                    kill(executable_commands[j].pid);
                }
            }
            return ERROR;
        }
    }
    if (foreground) {
		for (int i = 0; i < executable_command_count; i++) {
			if (executable_commands[i].pid != -1) {
				waitPid(executable_commands[i].pid);
			}
		}
		if (pipe_pos) {
			destroyPipe(executable_commands[0].fds[1]);
		}
	}

	return OK;
}

void initShell() {
    askForUser();
    clearTerminal();
    printf(WELCOME_MESSAGE, username);
    showTime();
    putchar('\n');
    help();
    putchar('\n');
}

static void toUtcMinus3(time_struct * time) {
    if (time->hour < 3) {
        time->hour += 21;
        time->day--;
        if (time->day == 0) {
            time->month--;
            if (time->month == 0) {
                time->month = 12;
                time->year--;
            }
            if(time->month == 2){
                time->day = 28;
                if(time->year % 4 == 0) {
                    time->day = 29;
                }
            } else if(time->month == 4 || time->month == 6 || time->month == 9 || time->month == 11) {
                time->day = 30;
            } else {
                time->day = 31;
            }
        }
    }
    else {
        time->hour = time->hour - 3;
    }
}

int colorShowcase() {
    puts("=== Welcome to the Great Terminal Color Parade! ===\n");

    const char* descriptions[] = {
        "Silence in the court... here comes BLACK!",
        "Bright and proud, the noble WHITE arrives.",
        "Fiery and bold — it's RED! [FLAMES]",
        "Sprouting life — GREEN marches in. [LEAVES]",
        "Splashing waves — BLUE flows! [OCEAN]",
        "Like sunshine — YELLOW shines through. [SUN]",
        "Streaming through the sky — CYAN glides. [SKY]",
        "A burst of color — MAGENTA dazzles. [SPARK]",
        "Neutral observer — GRAY strolls by. [MIST]",
        "Tangerine vibes — make way for ORANGE! [CITRUS]",
        "A mystery in royalty — PURPLE glimmers. [CLOAK]",
        "Down-to-earth charm — BROWN trudges by. [WOOD]",
        "Sweet and soft — PINK prances in. [CANDY]",
        "So bright it bites — LIME lights up! [ZEST]",
        "Dark and deep — NAVY sails past. [ANCHOR]",
        "Quiet and cool — TEAL tiptoes in. [WAVE]",
        "Old school — OLIVE nods wisely. [LEAF]",
        "Passion and power — MAROON roars. [BEAT]",
        "Polished and chic — SILVER sparkles. [GEM]",
        "Regal and radiant — GOLD concludes the parade! [CROWN]"
    };

    for (int i = STDIN; i < FDS_COUNT; i++) {
        printf("[*] ");
        fdprintf(i, descriptions[i - STDIN]);
        putchar('\n');
    }

    puts("\n=== End of Color Parade ===");
    puts("Use these color codes to make your shell stylish and expressive.");
    return OK;
}