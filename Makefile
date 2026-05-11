# DSP (Digital Signal Processing)

MAJOR		:= 2
MINOR		:= 1
PATCH		:= 0

# Common program utilities
CC			:= gcc
AR			:= ar rcs
RM			:= rm -rf

# Common flags for the compiler
CFLAGS	:= -O1 -march=native
DEPENDS	:= $(shell pkg-config --cflags --libs gtk4)

# Source files
SOURCES	:= $(wildcard ./src/*.c)
OBJECTS	:= $(notdir $(patsubst %.c,%.o,$(SOURCES)))

# Example programs
MAIN 		:= ./examples/main.c
BINARY	:= ./examples/main

# Static library
LIBRARY	:= dsp
LIB_DIR	:= ./lib

.PHONY	:= all

all:
	$(CC) $(SOURCES) -c $(CFLAGS) $(DEPENDS)
	$(AR) $(LIB_DIR)/lib$(LIBRARY).a $(OBJECTS)
	$(RM) $(OBJECTS)

	$(CC) $(MAIN) $(CFLAGS) -L$(LIB_DIR) -l$(LIBRARY) \
		$(DEPENDS) -o $(BINARY)
