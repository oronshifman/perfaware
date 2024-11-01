#!/bin/bash
if [ "$1" = "all" ] || [ "$1" = "generator" ]; then
    echo "+==================+"
    echo "+ Making generator +"
    echo "+==================+"
    make -C haversine_generator/; echo
elif [ "$1" = "all" ] || [ "$1" = "parser" ]; then
    echo "+===================+"
    echo "+ Making JSONParser +"
    echo "+===================+"
    make -C ../JSORON/JSONParser/ debug; echo
    make -C ../JSORON/JSONParser/ release; echo
elif [ "$1" = "all" ] || [ "$1" = "obj" ]; then
    echo "+===================+"
    echo "+ Making JSONObject +"
    echo "+===================+"
    make -C ../JSORON/JSONObject/ debug; echo
    make -C ../JSORON/JSONObject/ release; echo
elif [ "$1" = "all" ] || [ "$1" = "profiler" ]; then
    echo "+=================+"
    echo "+ Making profiler +"
    echo "+=================+"
    make -C profiler/ debug; echo
    make -C profiler/ release; echo
elif [ "$1" = "all" ] || [ "$1" = "calc" ]; then
    echo "+===================+"
    echo "+ Making calculator +"
    echo "+===================+"
    make -C haversine_calc/ debug; echo
    # make -C haversine_calc/ release; echo 
    make -C haversine_calc/ profiled; 
fi

# echo "+=============================================+"
# echo "+ Compiling simple_haversine and dependencies +"
# echo "+=============================================+"
# echo casey_calc_debug/release
# pushd haversine_calc/build
# g++ -g3 -o casey_calc_debug ../../casey_code/listing_0067_simple_haversine_main.cpp
# g++ -DNDEBUG -O3 -o casey_calc_release ../../casey_code/listing_0067_simple_haversine_main.cpp
# popd; echo

# echo "+=====================================+"
# echo "+ Compiling simple_haversine profiled +"
# echo "+=====================================+"
# echo profiled_calc_debug/release
# pushd haversine_calc/build
# g++ -DWITH_CASEY -g3 -o profiled_calc_debug -I../../profiler/include -I../../utils ../../profiler/src/profiler.cpp ../test/haversine_calc_v2_main.cpp
# g++ -DWITH_CASEY -DNDEBUG -O3 -o profiled_calc_release -I../../profiler/include -I../../utils ../../profiler/src/profiler.cpp ../test/haversine_calc_v2_main.cpp
# popd