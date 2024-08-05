TEST_OBJ_OBJS = src/JSONObject.o test/test_JSONObject.o
TEST_OBJ_ELF = build/test_JSONObject

TEST_VALUE_OBJ = src/_JSORONValue.o test/test_JSORONValue.o
TEST_VALUE_ELF = build/test_JSORONValue

TEST=../../utils/generic_test.o

CXX=g++

CXXFLAGS=-Wall -std=c++17

CPPFLAGS=-Iinclude -I../../utils

RELEASE_FLAGS=-DNDEBUG -O3
DEBUG_FLAG=-g3

all: 

test_value: CXXFLAGS+=$(DEBUG_FLAG)
test_value: $(TEST_VALUE_ELF)

test_obj: CXXFLAGS+=$(DEBUG_FLAG)
test_obj: $(TEST_OBJ_ELF)

$(TEST_OBJ_ELF): $(TEST_OBJ_OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(TEST_OBJ_ELF) $(TEST_OBJ_OBJS) $(TEST)

$(TEST_VALUE_ELF): $(TEST_VALUE_OBJ) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(TEST_VALUE_ELF) $(TEST_VALUE_OBJ) $(TEST)

clean:
	rm build/test_* src/*.o test/*.o
