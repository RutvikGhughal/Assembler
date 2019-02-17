TOPDIR := ./
SRCDIR := $(TOPDIR)"Program_Files"/
BINDIR := $(TOPDIR)

SOURCES := $(shell find $(SRCDIR) -name "*.c")

CC := gcc 

all: assembler

assembler: $(SOURCES)
	$(CC) $^ -o $(BINDIR)/$@

clean:
	rm -rf *.o 
