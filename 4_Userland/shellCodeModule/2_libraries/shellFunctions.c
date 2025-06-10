// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "shellFunctions.h"

static void toUtcMinus3(time_struct * time);

static char username[MAX_USERNAME_LENGTH] = { 0 };

int help(int argc, char *argv[]);
static int opCode_command(int argc, char *argv[]);
static int divZero_command(int argc, char *argv[]);
int showTime();
int clearTerminal();
int getRegs();
int song_player();
int isValidBase(const char *base);
int isNumberInBase(const char *num, const char *base);
int isConvertValid(char words[MAX_WORDS][MAX_WORD_LENGTH]);
int colorShowcase();
int loop(int argc, char *argv[]);
int killCommand(int argc, char *argv[]);
static int nice_command(int argc, char *argv[]);
static int blockUnblock(int argc, char *argv[]);

static int cat(int argc, char *argv[]);
static int wc(int argc, char *argv[]);
static int filter(int argc, char *argv[]);

static uint8_t foreground = 1;

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
    {"opcode", 1, &opCode_command},
    {"divzero", 1, &divZero_command},
    {"testmm", 0, (EntryPoint) &testMM},
    {"ts", 0, (EntryPoint) &testProcess},
    {"tp", 0, (EntryPoint) &testPriority},
    {"tsem", 0, (EntryPoint)&test_sync},
    {"colorshow", 1, &colorShowcase},
    {"ps", 0, (EntryPoint) &ps},
    {"mem", 0, (EntryPoint) &mem},
    {"loop", 0, (EntryPoint) &loop},
    {"kill", 1, &killCommand},
    {"nice", 1, &nice_command},
    {"block", 1, &blockUnblock},
    {"cat", 0, (EntryPoint) &cat},
    {"wc", 0, (EntryPoint) &wc},
    {"filter", 0, (EntryPoint) &filter},
    {"phylo", 0, (EntryPoint) &phylo},
};

#define NUM_MODULES (sizeof(modules) / sizeof(modules[0]))

int help(int argc, char *argv[]) {
    if (argc == 0) {
        puts("Available Commands");
        puts("  help            - Shows all available commands or help <category>.");
        puts("  time            - Display the current system time.");
        puts("  clear           - Clear the terminal screen.");
        puts("  kill <pid>      - Kills the process with the given pid.");
        puts("  nice            - <pid> <priority> Changes the priority of the process with the given pid.");
        puts("  block <pid>     - Switches from blocked to ready state or vice versa.");
        puts("  mem             - Show memory information.");
        puts("  ps              - Show the process status.");
        puts("  loop <ms>       - Loop of ID and greeting every couple of 100*ms.");
        puts("  cat             - Displays the contents of a file.");
        puts("  wc              - Counts the number of lines, words, and characters in a file.");
        puts("  filter <lang>   - Filters the contents of a file based on a pattern.");
        puts("Type 'help util', 'help test', or 'help fun' for more.");
        return OK;
    }
    if (strcmp(argv[0], "util") == 0) {
        puts("  setfont <scale> - Adjust the font size. (1 or 2)");
        puts("  convert         - <base_1> <base_2> <number>");
        puts("  time            - Display the current system time.");
        return OK;
    }
    if (strcmp(argv[0], "test") == 0) {
        puts("  getregs         - Display the registers's contents.");
        puts("  divzero         - Throws division by zero exc.");
        puts("  opcode          - Throws invalid opcode exc.");
        puts("  testmm <mem>    - Test the memory manager.");
        puts("  ts <proc>       - Test the scheduler manager.");
        puts("  tp              - Test the priority manager.");
        puts("  tsem <n> <use>  - Test the semaphore manager.");
        return OK;
    }
    if (strcmp(argv[0], "fun") == 0) {
        puts("  snake           - Starts the Snake game.");
        puts("  spotify         - Starts the spotify (clone) interface.");
        puts("  piano           - Starts the piano interface.");
        puts("  colorshow       - Show the color showcase.");
        return OK;
    }
    puts("Unknown help category. Available: util, test, fun.");
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
        return OK;
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
        if (pipe_pos) {
            command = pipe_pos + 1;
        }
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
                    return modules[j].function(executable_commands[i].argc, executable_commands[i].args);
                } else {
                    executable_commands[i].pid = newProcess(
                        modules[j].function, executable_commands[i].args,
                        executable_commands[i].command, 0, executable_commands[i].fds);
                    if (executable_commands[i].pid == -1) {
                        fdprintf(STDERR, "Error creating process for command: %s\n",  executable_commands[i].command);
                        return ERROR;
                    }
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
    char *emptyArgv[] = {NULL};
    help(0, emptyArgv);
    putchar('\n');
}

static int opCode_command(int argc, char *argv[]) {
    opCode();
    return OK;
}

static int divZero_command(int argc, char *argv[]) {
    divZero();
    return OK;
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
        fdprintf(i, descriptions[i]);
        putchar('\n');
    }

    puts("\n=== End of Color Parade ===");
    return OK;
}

int loop(int argc, char *argv[]) {
    if (argc != 1) {
        fdprintf(STDERR, "Usage: loop <seconds>\n");
        return ERROR;
    }

    int seconds = satoi(argv[0], NULL);
    if (seconds <= 0) {
        fdprintf(STDERR, "Invalid number of seconds.\n");
        return ERROR;
    }
    while (1) {
        printf("PID: %d, Please kill me, im trapped in here\n", getPid());
        sleep(seconds * 100);
    }
    return OK;
}

int killCommand(int argc, char *argv[]) {
    if (argc != 1) {
        fdprintf(STDERR, "Invalid amount of arguments. kill <pid>\n");
		return ERROR;
	}
    int64_t pid = atoi(argv[0]);
	int ret = kill(pid);
	if (ret == -1) {
        fdprintf(STDERR, "Error killing process with PID %d.\n", pid);
		return ERROR;
	}
    fdprintf(STDMARK, "Process with PID %d killed successfully.\n", pid);
	return OK;
}

static int nice_command(int argc, char *argv[]) {
	if (argc != 2) {
        fdprintf(STDERR, "Invalid amount of arguments. Usage: nice <pid> <priority>\n");
		return ERROR;
	}
	int pid = atoi(argv[0]);
	int priority = atoi(argv[1]);
	if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        fdprintf(STDERR, "Invalid priority. Must be between %d and %d.\n", MIN_PRIORITY, MAX_PRIORITY);
		return ERROR;
	}
	int ret = nice(pid, priority);
	if (ret == -1) {
        fdprintf(STDERR, "Error changing priority for PID %d to %d.\n", pid, priority);
		return ERROR;
	}
	return OK;
}

static int blockUnblock(int argc, char *argv[]) {
    if (argc != 1) {
        fdprintf(STDERR, "Invalid amount of arguments. Usage: block <pid>\n");
        return ERROR;
    }
    int pid = atoi(argv[0]);
    process_info_t *process_list = (process_info_t *) sys_ps();
    process_info_t *current = process_list;
    while (current->pid != pid) {
        current++;
    }

    if (current->status == READY) {
        int ret = blockProcess(pid);
        if (ret == -1) {
            fdprintf(STDERR, "Error blocking process with PID %d.\n", pid);
            return ERROR;
        }
        fdprintf(STDMARK, "Process with PID %d changed from READY to BLOCKED.\n", pid);
    } else if (current->status == BLOCKED) {
        int ret = unblockProcess(pid);
        if (ret == -1) {
            fdprintf(STDERR, "Error unblocking process with PID %d.\n", pid);
            return ERROR;
        }
        fdprintf(STDMARK, "Process with PID %d changed from BLOCKED to READY.\n", pid);
    }
    
    return OK;
}

static int cat(int argc, char **argv) {
	if (argc != 0) {
		printf("cat: Invalid amount of arguments.\n");
		return -1;
	}

	int c;
	while ((c = getCharNoWait()) != EOF) {
		if (c)
			putchar(c);
	}

	return 0;
}

static int wc(int argc, char *argv[]) {
    char c;
    int lines = 0, words = 0, chars = 0;
    while((c = getCharNoWait()) != EOF) {
        if(c){
            putchar(c);
            chars++;
        }
        if(c == '\n') {
            lines++;
        }
        if(c == ' ') {
            words++;
        }
    }
    fdprintf(ORANGE, "\nReached EOF.\n");
    fdprintf(BLUE, "Lines: %d, Words: %d, Characters: %d\n", lines, words + 1, chars);
    return OK;
}

static bool isStandardVowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == '\n');
}

static bool isFrenchVowel(char c) {
    return isStandardVowel(c) || c == 'y' || c == 'Y';
}

static bool (*getVowelChecker(const char *lang))(char) {
    if (lang != NULL && strcmp(lang, "fr") == 0)
        return &isFrenchVowel;
    return &isStandardVowel;
}

static int filter(int argc, char *argv[]) {
    if(argc > 1) {
        fdprintf(STDERR, "Usage: filter optional: <fr>\n");
        return ERROR;
    }
    char c;
    char vocales[BUFFER_SIZE];
    int idx = 0;

    bool (*isVowel)(char) = getVowelChecker((argc == 1) ? argv[0] : NULL);

    while((c = getCharNoWait()) != EOF) {
        if(c) {
            putchar(c);
            if(isVowel(c) || (c == ' ' && idx > 0 && vocales[idx - 1] != ' ')) {
                vocales[idx++] = c;
            } else if (c == '\b' && idx > 0 && isVowel(vocales[idx - 1])) {
                idx--;
            }
        }
    }
    vocales[idx] = '\0';
    fdprintf(ORANGE, "\nReached EOF.\n");
    if (argc == 1 && strcmp(argv[0], "fr") == 0) {
        fdprintf(BLUE, "Vocales (fr): %s\n", vocales);
    } else {
        fdprintf(BLUE, "Vocales: \n%s\n", vocales);
    }
    return OK;
}