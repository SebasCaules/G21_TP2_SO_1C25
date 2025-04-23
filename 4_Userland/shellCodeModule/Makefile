include ../Makefile.inc

MODULE=0000-shellCodeModule.bin

# Source files, but excluding _loader.c from SOURCES
SOURCES=$(filter-out _loader.c, $(wildcard */*.c))
SOURCES_ASM=$(wildcard */*.asm)

# Object files
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c
LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

# Link the binary, ensuring _loader.o is linked only once
$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(LOADEROBJECT)
	$(LD) $(LDFLAGS) -T shellCodeModule.ld -o ../$(MODULE) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)

# Rule for compiling C files
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./1_include -c $< -o $@

# Rule for assembling ASM files
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Rule for compiling _loader.c
$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)

# Clean the build
clean:
	rm -rf */*.o *.o *.bin

.PHONY: all clean print