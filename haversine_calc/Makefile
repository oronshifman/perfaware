TARGET_NO_PROF = haversine_calc
TARGET_PROF = haversine_calc_profiled

OBJS = src/haversine_calc.o ../JSORON/JSONObject/src/JSONObject.o ../JSORON/JSONParser/src/JSONParser.o ../utils/listing_0065_haversine_formula.o

MAIN_OBJ_NO_PROF=test/$(TARGET_NO_PROF)_main.o
MAIN_OBJ_PROF=test/$(TARGET_PROF)_main.o

TEST=../utils/generic_test.o

RELEASE_BIN = build/$(TARGET_NO_PROF)_release
DEBUG_BIN = build/$(TARGET_NO_PROF)_debug
PROFILED_BIN = build/$(TARGET_PROF)_debug

CXX=g++

CXXFLAGS=-Wall -std=c++11

CPPFLAGS=-Iinclude -I../haversine_generator/include -I../JSORON/JSONObject/include -I../JSORON/JSONParser/include -I../utils

RELEASE_FLAGS=-DNDEBUG -O3
DEBUG_FLAG=-g3
PROFILING_FLAGS=-DPROFILING

all:

profiled: CXXFLAGS+=$(DEBUG_FLAG) $(PROFILING_FLAGS)
profiled: CPPFLAGS+=-I../profiler/include
profiled: OBJS+=../profiler/src/profiler.o
profiled: $(PROFILED_BIN)

release: CXXFLAGS+=$(RELEASE_FLAGS)
release: $(RELEASE_BIN) 

debug: CXXFLAGS+=$(DEBUG_FLAG)
debug: $(DEBUG_BIN)

$(PROFILED_BIN): $(MAIN_OBJ_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(PROFILED_BIN) $(MAIN_OBJ_PROF) $(OBJS) $(TEST)

$(DEBUG_BIN): $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(DEBUG_BIN) $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)

$(RELEASE_BIN): $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(RELEASE_BIN) $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)

clean:
	-rm -f build/* src/*.o test/*.o; touch build/dummy.md	
