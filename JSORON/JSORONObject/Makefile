TEST_OBJ_OBJS = src/JSORONObject.cpp test/test_JSORONObject.cpp
OBJ_ELF_NAME = build/test_JSORONObject

TEST_VALUE_OBJ = src/JSORONValue.cpp test/test_JSORONValue.cpp
VALUE_ELF_NAME = build/test_JSORONValue

TEST = ../../utils/generic_test.cpp

CC = g++

COMPILER_FLAGS = 

INCLUDE_DIR = -Iinclude -I../../utils

DEBUG_FLAG = -g

OBJ_NAME = build/JSORONObject

all: test_value test_obj
	
test_value: $(TEST_VALUE_OBJ) $(TEST)
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAG) -o $(VALUE_ELF_NAME) $(TEST_VALUE_OBJ) $(INCLUDE_DIR)

test_obj: $(TEST_OBJ_OBJS) $(TEST)
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAG) -o $(OBJ_ELF_NAME) $(TEST_OBJ_OBJS) $(INCLUDE_DIR)
