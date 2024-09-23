TARGET_NO_PROF = haversine_calc
TARGET_PROF = haversine_calc_profiled

TARGET_CASY = casy_calc

ORON_JSON = src/haversine_calc.o ../JSORON/JSONObject/src/JSONObject.o ../JSORON/JSONParser/src/JSONParser.o ../utils/listing_0065_haversine_formula.o
CASY_JSON = ../casy_code/listing_0067_simple_haversine_main.cpp

MAIN_OBJ_NO_PROF=test/$(TARGET_NO_PROF)_main.o
MAIN_OBJ_PROF=test/$(TARGET_PROF)_main.o

TEST=../utils/generic_test.o

RELEASE_BIN = build/$(TARGET_NO_PROF)_release
DEBUG_BIN = build/$(TARGET_NO_PROF)_debug
PROFILED_BIN = build/$(TARGET_PROF)_debug

CASY_DEBUG_BIN = build/$(TARGET_CASY)_debug
CASY_RELEASE_BIN = build/$(TARGET_CASY)_release

CXX=g++

CXXFLAGS=-Wall -std=c++11

CPPFLAGS=-Iinclude -I../haversine_generator/include -I../utils -I../JSORON/JSONObject/include -I../JSORON/JSONParser/include

RELEASE_FLAGS=-DNDEBUG -O3
DEBUG_FLAG=-g3
PROFILING_FLAGS=-DPROFILING

all:

casy_debug: $(CASY_DEBUG_BIN)
casy_release: $(CASY_RELEASE_BIN)

profiled: CXXFLAGS+=$(DEBUG_FLAG) $(PROFILING_FLAGS)
profiled: CPPFLAGS+=$(ORON_JSON_INCLUDES) -I../profiler/include
profiled: OBJS+=../profiler/src/profiler.o
profiled: $(PROFILED_BIN)

release: CXXFLAGS+=$(ORON_JSON_INCLUDES) $(RELEASE_FLAGS)
release: $(RELEASE_BIN) 

debug: CXXFLAGS+=$(ORON_JSON_INCLUDES) $(DEBUG_FLAG)
debug: $(DEBUG_BIN)

$(CASY_DEBUG_BIN): $(CASY_JSON)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAG) -o $(CASY_DEBUG_BIN) $(CASY_JSON)

$(CASY_RELEASE_BIN): $(CASY_JSON)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $(CASY_RELEASE_BIN) $(CASY_JSON)

$(PROFILED_BIN): $(MAIN_OBJ_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(PROFILED_BIN) $(MAIN_OBJ_PROF) $(OBJS) $(TEST)

$(DEBUG_BIN): $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(DEBUG_BIN) $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)

$(RELEASE_BIN): $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(RELEASE_BIN) $(MAIN_OBJ_NO_PROF) $(OBJS) $(TEST)

clean:
	-rm -f build/* src/*.o test/*.o; touch build/dummy.md	
