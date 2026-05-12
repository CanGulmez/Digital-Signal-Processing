# Compiler & archiver
CC      := gcc
AR      := ar rcs
RM      := rm -rf

# Compilation flags
CFLAGS  := -O1 -march=native
DEPENDS := -lm $(shell pkg-config --cflags --libs gtk4 gsl)

# Source files
SOURCES := $(wildcard ./src/*.c)
OBJECTS := $(notdir $(patsubst %.c,%.o,$(SOURCES)))

# Example programs
MAIN    := $(wildcard ./examples/*.c)
BINARY  := ./bin/main

# Static library
LIBRARY := dsp
LIB_DIR := ./lib

.PHONY: all

all:
	@echo "--> Compiling the library source files..."
	$(CC) $(SOURCES) -c $(CFLAGS) $(DEPENDS)

	@echo "\n--> Archiving the static library..."
	$(AR) $(LIB_DIR)/lib$(LIBRARY).a $(OBJECTS)

	@echo "\n--> Deleting the unnecessary object files..."
	$(RM) $(OBJECTS)

	@echo "\n--> Compiling the example program..."
	$(CC) $(MAIN) $(CFLAGS) -L$(LIB_DIR) -l$(LIBRARY) $(DEPENDS) -o $(BINARY)
