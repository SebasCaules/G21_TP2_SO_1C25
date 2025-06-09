include Makefile.inc

KERNEL=kernel.bin

ifdef MM
    MM_FLAG := -D$(MM)
else
    MM_FLAG :=
endif

# Define source directories
ASM_DIR=0_asm
INCLUDE_DIR=1_include
LIB_DIR=2_libraries
DRIVERS_DIR=3_drivers
INTERRUPTIONS_DIR=4_interruptions

# Source files
ROOT_SOURCES=$(wildcard *.c)
DRIVER_SOURCES=$(wildcard $(DRIVERS_DIR)/*.c)
INTERRUPTIONS_SOURCES=$(wildcard $(INTERRUPTIONS_DIR)/*.c)
LIB_SOURCES=$(wildcard $(LIB_DIR)/*.c)
SOURCES_ASM=$(wildcard $(ASM_DIR)/*.asm)

# Object files
ROOT_OBJECTS=$(patsubst %.c,%.o,$(ROOT_SOURCES))
DRIVER_OBJECTS=$(patsubst $(DRIVERS_DIR)/%.c,$(DRIVERS_DIR)/%.o,$(DRIVER_SOURCES))
INTERRUPTIONS_OBJECTS=$(patsubst $(INTERRUPTIONS_DIR)/%.c,$(INTERRUPTIONS_DIR)/%.o,$(INTERRUPTIONS_SOURCES))
LIB_OBJECTS=$(patsubst $(LIB_DIR)/%.c,$(LIB_DIR)/%.o,$(LIB_SOURCES))
OBJECTS_ASM=$(patsubst $(ASM_DIR)/%.asm,$(ASM_DIR)/%.o,$(SOURCES_ASM))

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

# Target
all: $(KERNEL)

# Link the binary
$(KERNEL): $(LOADEROBJECT) $(ROOT_OBJECTS) $(DRIVER_OBJECTS) $(INTERRUPTIONS_OBJECTS) $(LIB_OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(ROOT_OBJECTS) $(DRIVER_OBJECTS) $(INTERRUPTIONS_OBJECTS) $(LIB_OBJECTS) $(OBJECTS_ASM)

# Compile C files in root
%.o: %.c
	$(GCC) $(GCCFLAGS) -I$(INCLUDE_DIR) -I../6_NoMansLand -c $< -o $@

# Compile C files in drivers
$(DRIVERS_DIR)/%.o: $(DRIVERS_DIR)/%.c
	$(GCC) $(GCCFLAGS) -I$(INCLUDE_DIR) -I../6_NoMansLand -c $< -o $@

# Compile C files in interruptions
$(INTERRUPTIONS_DIR)/%.o: $(INTERRUPTIONS_DIR)/%.c
	$(GCC) $(GCCFLAGS) -I$(INCLUDE_DIR) -I../6_NoMansLand -c $< -o $@

# Compile C files in libraries
$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	$(GCC) $(GCCFLAGS) -I$(INCLUDE_DIR) -I../6_NoMansLand -c $< -o $@ $(MM_FLAG)

# Assemble ASM files
$(ASM_DIR)/%.o: $(ASM_DIR)/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Assemble loader.asm (which is in the root directory)
$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# Clean the build
clean:
	rm -rf $(ASM_DIR)/*.o $(DRIVERS_DIR)/*.o $(INTERRUPTIONS_DIR)/*.o $(LIB_DIR)/*.o *.o *.bin

.PHONY: all clean