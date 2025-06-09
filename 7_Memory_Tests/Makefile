CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -I../6_NoMansLand
LDFLAGS =

# Archivos fuente
SRCS = test_mm.c memoryManager.c
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
TARGET = mmTest

# Compilación por defecto
all: $(TARGET)

# Linkeo del ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean