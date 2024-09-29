TARGET=haversine_calc

OBJS=src/haversine_calc.o test/haversine_calc_main.o ../profiler/src/profiler.o ../../JSORON/JSONParser/src/JSONParser.o ../../JSORON/JSONObject/src/JSONObject.o ../utils/listing_0065_haversine_formula.o

TEST=../utils/generic_test.o

RELEASE_BIN = build/$(TARGET)_release
DEBUG_BIN = build/$(TARGET)_debug

CXX=g++

CXXFLAGS=-Wall -std=c++11

CPPFLAGS=-Iinclude -I../utils -I../profiler/include -I../../JSORON/JSONObject/include -I../../JSORON/JSONParser/include

RELEASE_FLAGS=-DNDEBUG -O3
DEBUG_FLAG=-g3

all: debug release

release: CXXFLAGS+=$(RELEASE_FLAGS)
release: $(RELEASE_BIN) 

debug: CXXFLAGS+=$(DEBUG_FLAG)
debug: $(DEBUG_BIN)

$(DEBUG_BIN): $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(DEBUG_BIN) $(OBJS) $(TEST)

$(RELEASE_BIN): $(OBJS) $(TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(RELEASE_BIN) $(OBJS) $(TEST)

clean:
	-rm -f build/* src/*.o test/*.o; touch build/dummy.md	