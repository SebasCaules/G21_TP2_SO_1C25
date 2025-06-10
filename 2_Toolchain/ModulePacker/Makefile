MP=mp.bin
SOURCES=$(wildcard *.c)

ifdef MM
	CFLAGS += -D$(MM)
endif

all: $(MP)

$(MP): $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $(MP)

clean:
	rm -rf $(MP)

.PHONY: all clean
