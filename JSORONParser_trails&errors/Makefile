OBJS = src/JSORONObject.cpp test/JSORONParser_main.cpp

CC = g++

COMPILER_FLAGS = 

INCLUDE_DIR = -Iinclude -I../utils

DEBUG_FLAG = -g

OBJ_NAME = build/JSORONParser

all: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAG) -o $(OBJ_NAME) $(OBJS) $(INCLUDE_DIR)
	
