TARGET = haversine_calc

OBJS = src/haversine_calc.o ../JSORON/JSONObject/src/JSONObject.o ../JSORON/JSONParser/src/JSONParser.o ../utils/listing_0065_haversine_formula.o

MAIN_OBJ=test/$(TARGET)_main.o

TEST=../utils/generic_test.o

RELEASE_BIN = build/$(TARGET)_release
DEBUG_BIN = build/$(TARGET)_debug
PROFILED_BIN = build/$(TARGET)_profiled_debug

CXX=g++

CXXFLAGS=-Wall -std=c++11

CPPFLAGS=-Iinclude -I../haversine_generator/include -I../JSORON/JSONObject/include -I../JSORON/JSONParser/include -I../utils

RELEASE_FLAGS=-DNDEBUG -O3
DEBUG_FLAG=-g3

all:

try:
	echo $(TARGET)

profiled: TARGET=haversine_calc_profiled
profiled: CXXFLAGS+=$(DEBUG_FLAG)
profiled: CPPFLAGS+=-I../profiler/include
profiled: OBJS+=../profiler/src/profiler.o
profiled: $(PROFILED_BIN)

release: CXXFLAGS+=$(RELEASE_FLAGS)
release: $(RELEASE_BIN) 

debug: CXXFLAGS+=$(DEBUG_FLAG)
debug: $(DEBUG_BIN)

$(PROFILED_BIN): test/haversine_calc_profiled_main.o $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(PROFILED_BIN) $(MAIN_OBJ) $(OBJS) $(TEST)

$(DEBUG_BIN): $(MAIN_OBJ) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(DEBUG_BIN) $(MAIN_OBJ) $(OBJS) $(TEST)

$(RELEASE_BIN): $(MAIN_OBJ) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(RELEASE_BIN) $(MAIN_OBJ) $(OBJS) $(TEST)

clean:
	-rm -f build/* src/*.o test/*.o; touch build/dummy.md	
