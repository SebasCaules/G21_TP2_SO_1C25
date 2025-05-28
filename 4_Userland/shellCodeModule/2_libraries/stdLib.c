#include <stdLib.h>

// MSJ_1 : Debido a un error con el stdarg.h y los flags -mno-sse y -mno-sse2 no se pueden implementar los doubles o floats

static uint64_t typeToBuffer(char* buffer, uint64_t length, va_list args, Types type){
	uint64_t ret = length;
	char auxBuffer[BUFFER_SIZE];
	uint64_t auxLength = 0;
	char numberFlag = 1;

	switch (type) {
	case INT_TYPE:
		auxLength = convert_to_base_string(va_arg(args, int), 10, auxBuffer);
		break;
    case HEX_TYPE:
        auxLength = convert_to_base_string(va_arg(args, uint64_t), 16, auxBuffer);
        break;
    case BIN_TYPE:
        auxLength = convert_to_base_string(va_arg(args, int), 2, auxBuffer);
        break;
    case OCT_TYPE:
        auxLength = convert_to_base_string(va_arg(args, int), 8, auxBuffer);
        break;
		break;
	case STR_TYPE:
		char* str = va_arg(args, char*);
		while (*str) {
			buffer[ret++] = *str;
			str++;
		}
        numberFlag = 0;
		break;
    // MSJ_1
	// case DOUBLE_TYPE:
	// 	auxLength = decimalToString(va_arg(args, double), auxBuffer);
	// 	break;
	// case FLOAT_TYPE:
	// 	auxLength = decimalToString((float)va_arg(args, double), auxBuffer);
	default:
		break;
	}
    
	if(numberFlag){
		for (uint8_t i = 0; i < auxLength; i++) {	
			buffer[ret++] = auxBuffer[i];
		}
	}
	return ret;
}

static uint64_t argsToBuffer(char* buffer, const char* fmt, va_list args) {
    uint64_t length = 0;
    for (uint64_t i = 0; fmt[i]; i++) {
        char fmtSpecifier;
        if(fmt[i] == '%' && (fmtSpecifier = fmt[i + 1]) != '\0') {
            switch (fmtSpecifier) {
            case 'd':
				length = typeToBuffer(buffer, length, args, INT_TYPE);
                break;
            case 'x':
				length = typeToBuffer(buffer, length, args, HEX_TYPE);
                break;
                break;
            case 'c': 
                buffer[length++] = (char)va_arg(args, int);
                break;
            case 's':
				length = typeToBuffer(buffer, length, args, STR_TYPE);
                break;
            //MSJ_1
            // case 'f':
			// 	length = typeToBuffer(buffer, length, args, FLOAT_TYPE);
            //     break;
            // case 'g':
			// 	length = typeToBuffer(buffer, length, args, DOUBLE_TYPE);
            default:
                break;
            }
            i++;
        } else {
            buffer[length++] = fmt[i];
        }
    }
    return length;
}

uint64_t vargsToBuffer(char* buffer, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    uint64_t length = argsToBuffer(buffer, fmt, args);
    va_end(args);
    return length;
}

static int printArgs(uint64_t fd, const char* fmt, va_list args) {
    char buffer[BUFFER_SIZE];
    uint64_t length = argsToBuffer(buffer, fmt, args);
    return sys_write(fd, (uint16_t *)buffer, length);
}

int fdprintf(uint64_t fd, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int toRet = printArgs(fd, fmt, args);
    va_end(args);
    return toRet;
}

int	printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int toRet = printArgs(STDOUT, fmt, args);
    va_end(args);
    return toRet;
}

int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = 0;
    char buffer[BUFFER_SIZE];
    int index = 0;

    while (sys_read(STDIN, (uint16_t *)&buffer[index], 1) > 0 && buffer[index] != '\n') {
        index++;
    }
    buffer[index] = '\0'; 

    const char *fmt = format;
    int *int_ptr;
    char *fmt_ptr;
    int i = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') { 
                int_ptr = va_arg(args, int *);
                *int_ptr = 0;
                while (buffer[i] >= '0' && buffer[i] <= '9') {
                    *int_ptr = *int_ptr * 10 + (buffer[i++] - '0');
                }
                result++;
            } else if (*fmt == 's') {
                fmt_ptr = va_arg(args, char *);
                int j = 0;
                while (buffer[i] && buffer[i] != ' ') {
                    fmt_ptr[j++] = buffer[i++];
                }
                fmt_ptr[j] = '\0';
                result++;
            }
        } else if (*fmt != buffer[i++]) {
            break;
        }
        fmt++;
    }
    va_end(args);
    return result;
}

int getchar() {
    uint16_t c;
    while (sys_read(STDIN, &c, 1) == 0 || c > 255);
    return (char) c;
}

int readInput(char * c) {
    return sys_read(STDIN, (uint16_t *)c, 1);
}

int putchar(char c) {
    return sys_write(STDOUT, (uint16_t *)&c, 1);
}

int putsNoNewLine(const char *fmt) {
    int len;
    while (*fmt) {
        putchar(*fmt++);
        len++;
    }
    return len;
}

int puts(const char *fmt) {
    putsNoNewLine(fmt);
    return putchar('\n');
}

int gets(char* buffer, int n) {
    int c;
    int i = 0;

    while ((c = getchar()) != '\n') {
        if (c == '\b' && i > 0) {
            putchar(c);
            i--;
        }
        if (c != '\b' && i < n - 1) {
            putchar(c);
            buffer[i++] = (char)c;
        }
    }
    putchar('\n');
    buffer[i] = '\0';

    return i;
}

int setFontScale(int scale) {
    return sys_set_font_scale(scale);
}

void clearView() {
    sys_clear();
}

void drawRectangle(int x, int y, int width, int height, int color) {
    sys_draw_rectangle(x, y, width, height, color);
}

void sleep(int milliseconds) {
    sys_sleep(milliseconds);
}

void resetCursor() {
    sys_reset_cursor();
}

void beep(int freq, int milliseconds) {
    sys_beep(freq, milliseconds);
}

static int get_base_from_char(char base) {
    switch (base) {
        case 'b': return 2;
        case 'o': return 8;
        case 'd': return 10;
        case 'h': return 16;
        default: return -1;  // Invalid base
    }
}

static int to_decimal(const char *fmt, int base) {
    int result = 0;
    int i = 0;

    while (fmt[i] != '\0') {
        int digit;
        
        if (fmt[i] >= '0' && fmt[i] <= '9') {
            digit = fmt[i] - '0';
        }
        else if (fmt[i] >= 'A' && fmt[i] <= 'F') {
            digit = fmt[i] - 'A' + 10;
        }
        else if (fmt[i] >= 'a' && fmt[i] <= 'f') {
            digit = fmt[i] - 'a' + 10;
        } else {
            return -1;
        }

        if (digit >= base) {
            return -1;
        }

        result = result * base + digit;
        i++;
    }
    return result;
}

static void from_decimal(int decimal, int base, char *buffer) {
    const char *digits = "0123456789ABCDEF";
    int pos = 0;

    if (decimal == 0) {
        buffer[pos++] = '0';
        buffer[pos] = '\0';
        return;
    }

    while (decimal > 0) {
        buffer[pos++] = digits[decimal % base];
        decimal /= base;
    }
    
    buffer[pos] = '\0';

    for (int i = 0; i < pos / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[pos - i - 1];
        buffer[pos - i - 1] = temp;
    }
}

char* convert(char initBase, char finalBase, char *num) {
    int initBaseValue = get_base_from_char(initBase);
    int finalBaseValue = get_base_from_char(finalBase);
    static char bufferRet[BUFFER_SIZE];

    if (initBaseValue == -1 || finalBaseValue == -1) {
        return "The initial and final base must be one of: 'b', 'o', 'd', 'h'\n";
    }

    int decimal = to_decimal(num, initBaseValue);
    if (decimal == -1) {
        vargsToBuffer(bufferRet, "Invalid number %s for base %c\n", num, initBase);
        return bufferRet;
    }

    char convertedNum[BUFFER_SIZE];
    from_decimal(decimal, finalBaseValue, convertedNum);

    if(finalBaseValue == 2){
        vargsToBuffer(bufferRet, "Number %s in base %c: %sb\n", num, finalBase, convertedNum);
    }
    else if(finalBaseValue == 16){
        vargsToBuffer(bufferRet, "Number %s in base %c: 0x%s\n", num, finalBase, convertedNum);
    }
    else {
        vargsToBuffer(bufferRet, "Number %s in base %c is %s in base %c\n", num, initBase, convertedNum, finalBase);
    }
    return bufferRet;
}


//Cambiar opr StrToInt
int64_t satoi (char * str, int64_t * flag) {
	uint64_t i = 0;
	int64_t res = 0;
	int8_t sign = 1;

	if ( !str ) {
		if ( flag ) {
			*flag = 0;
		}
		return 0;
	}

	if ( str[i] == '-' ) {
		i++;
		sign = -1;
	}

	for ( ; str[i] != '\0'; ++i ) {
		if ( str[i] < '0' || str[i] > '9' ) {
			if ( flag ) {
				*flag = 0;
			}
			return 0;
		}

		res = res * 10 + str[i] - '0';
	}
	if ( flag ) {
		*flag = 1;
	}
	return res * sign;
}

void * my_malloc(uint64_t size) {
    return sys_my_malloc(size);
}

void my_free(void *ptr) {
    return sys_my_free(ptr);
}

int64_t newProcess(uint64_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors) {
    return sys_new_process(main, argv, name, unkillable, fileDescriptors);
}

int64_t exit(int64_t retValue) {
    return sys_exit(retValue);
}

int64_t getPid(void) {
    return sys_get_pid();
}

int64_t processStatus(void) {
    return sys_process_status();
}

int64_t kill(uint64_t pid) {
    return sys_kill_process((uint16_t)pid);
}

int64_t setPriority(uint16_t pid, uint8_t newPriority) {
    return sys_set_priority(pid, newPriority);
}

int64_t blockProcess(uint16_t pid) {
    return sys_block_process(pid);
}

int64_t unblockProcess(uint16_t pid) {
    return sys_unblock_process(pid);
}

int64_t yield(void) {
    return sys_yield();
}

int64_t waitPid(uint32_t pid) {
    return sys_waitpid(pid);
}

int64_t nice(uint64_t pid, uint64_t newPriority) {
	return setPriority(pid, newPriority);
}

int64_t semOpen(char *name, int initialValue) {
    return sys_sem_open(name, initialValue);
}

int64_t semClose(char *name) {
    return sys_sem_close(name);
}

int64_t semWait(char *name) {
    return sys_sem_wait(name);
}

int64_t semPost(char *name) {
    return sys_sem_post(name);
}