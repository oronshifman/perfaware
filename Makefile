OBJS = src/haversine_generator.cpp src/listing_0065_haversine_formula.cpp test/haversine_generator_main.cpp

CC = g++

COMPILER_FLAGS = 

INCLUDE_DIR = include

DEBUG_FLAG = -g

OBJ_NAME = build/haversine_generator

all: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAG) -o $(OBJ_NAME) $(OBJS) -I $(INCLUDE_DIR)
	
