# DSP (Digital Signal Processing)

MAJOR		:= 2
MINOR		:= 0
PATCH		:= 0

CC			:= gcc
AR			:= ar rcs
RM			:= rm -rf

CFLAGS	:= -O1 -march=native
DEPENDS	:= $(shell pkg-config --cflags --libs gtk4) -lm -lgsl

SOURCES	:= $(wildcard ./src/*.c)
OBJECTS	:= $(notdir $(patsubst %.c,%.o,$(SOURCES)))
MAIN 		:= ./bin/main.c
BINARY	:= ./bin/main
LIBRARY	:= dsp
LIB_DIR	:= ./lib

.PHONY	:= all

all:
	$(CC) $(SOURCES) -c $(CFLAGS) $(DEPENDS)
	$(AR) $(LIB_DIR)/lib$(LIBRARY).a $(OBJECTS)
	$(RM) $(OBJECTS)

	$(CC) $(MAIN) $(CFLAGS) -L$(LIB_DIR) -l$(LIBRARY) \
	$(DEPENDS) -o $(BINARY)
