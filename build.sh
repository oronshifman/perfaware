#!/bin/bash

echo "+==================+"
echo "+ Making generator +"
echo "+==================+"
make -C haversine_generator/; echo

# echo "+===================+"
# echo "+ Making JSONParser +"
# echo "+===================+"
# make -C JSORON/JSONParser/ debug; echo
# make -C JSORON/JSONParser/ release; echo

# echo "+===================+"
# echo "+ Making JSONObject +"
# echo "+===================+"
# make -C JSORON/JSONObject/ debug; echo
# make -C JSORON/JSONObject/ release; echo

echo "+=================+"
echo "+ Making profiler +"
echo "+=================+"
make -C profiler/ debug; echo
make -C profiler/ release; echo

# echo "+===================+"
# echo "+ Making calculator +"
# echo "+===================+"
# make -C haversine_calc/ debug; echo
# make -C haversine_calc/ release; echo
# make -C haversine_calc/ profiled

echo "+=============================================+"
echo "+ Compiling simple_haversine and dependencies +"
echo "+=============================================+"
pushd haversine_calc/build
g++ -g3 -o casy_calc_debug ../../casy_code/listing_0067_simple_haversine_main.cpp
g++ -DNDEBUG -O3 -o casy_calc_release ../../casy_code/listing_0067_simple_haversine_main.cpp
popd