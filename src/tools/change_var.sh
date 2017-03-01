#!/bin/sh

set -e

usage() {
    echo $1
    echo "Usage : sh change_vars.sh file var new_value*"
    echo "Modify the variable declared in file to get the new value"
    echo "file\tThe file to change"
    echo "var\tThe var to be modified"
    echo "new_value\tThe new value of the variable (can contain spaces)"
}

if [ $# -ge 2 ]
then
    file=$1
    var=$2
    shift 2
    nval="$@"
else
    usage "Not enough arguments ($@), at least 2 required"
    exit 1
fi

sed -i --posix "s:^export $var=.*$:export $var=\"$nval\":" $file
