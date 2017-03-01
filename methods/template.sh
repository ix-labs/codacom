#!/bin/sh

#For debug
#set -x
set -e 

usage() {
    echo $1
    echo "Usage : execute.sh [-d] file vars"
		echo "-d : does not execute the method, but outputs the descriptions"
    echo "file : graph file (position relative to the root dir) that serves as an input to the clustering method"
    echo "vars : a shell file containing the system-dependant variables (position of directories etc.)"
}

if [ $1 = "-d" ]
then
		echo "$DESCRIPTION"
		exit 0
fi

if [ $# -ge 2 ]
then
    INPUT_GRAPH=$1
    VARS=$2
else
    usage "Not enough arguments, 2 required"
    exit 1
fi

. $VARS

base=$(basename -s .txt $INPUT_GRAPH)
dir=$(basename $(pwd))
OUTPUT_CLUST=$OUTPUT_DIR/$base"_"$dir
