#!/bin/bash

##
## Auteur : Sébastien Le Maire
##

BINNAME="hathoria"
BUILD_DIRECTORY="./build"

if [ -e ${BUILD_DIRECTORY} ]
then
    rm -fr ${BUILD_DIRECTORY}
fi

mkdir -p ${BUILD_DIRECTORY}

cd ${BUILD_DIRECTORY}

if [ $# -ne 0 ] && [ "$1" == "release" ]
then
    cmake .. -G "Unix Makefiles" && cmake --build .
else
    cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug && cmake --build .
fi