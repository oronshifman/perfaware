#!/bin/bash

echo "+==================+"
echo "+ Making generator +"
echo "+==================+"
make -C haversine_generator/; echo

echo "+===================+"
echo "+ Making JSONParser +"
echo "+===================+"
make -C JSORON/JSONParser/ debug; echo
make -C JSORON/JSONParser/ release; echo

echo "+===================+"
echo "+ Making JSONObject +"
echo "+===================+"
make -C JSORON/JSONObject/ debug; echo
make -C JSORON/JSONObject/ release; echo

echo "+=================+"
echo "+ Making profiler +"
echo "+=================+"
make -C profiler/ debug; echo
make -C profiler/ release; echo

echo "+===================+"
echo "+ Making calculator +"
echo "+===================+"
make -C haversine_calc/ debug; echo
make -C haversine_calc/ release; echo
make -C haversine_calc/ profiled