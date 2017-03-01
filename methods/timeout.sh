#!/bin/sh

set -e

usage() {
    echo $1
    echo "Usage : timeout.sh time dir command"
    echo "Executes the given method, exits 15 if it reaches the designated time (note : stderr of the command is redirected to stdout)"
    echo "time : timeout (in s)"
    echo "dir : the dir where to set the execution environment to launch the command"
    echo "command : the executed command"
    exit 1
}

#set echo -noecho
#expect timeout { return 15 }
#catch wait result
#exit [lindex $result 3]
timeout_expect() {
export time="$1"
shift 1
expect -c "set timeout $time; spawn -noecho $@; expect timeout { exit 15 } expect eof; foreach {pid spawnid os_error_flag value} [wait] break; exit \$value"
return $?
}

if [ "$#" -gt 2 ]
then
    timeout=$1
    dir=$2
    shift 2
    command=$@
else
    usage "Not enough arguments"
fi

cd $dir
ret=0
#>&1 expect -c "\"set echo -noecho; set timeout $timeout; spawn -noecho $command; expect timeout { exit 15 } expect eof; catch wait result; exit [lindex \\\$result 3]\"" || ret=$
if [ $EXPECT = 0 ]
then
		>&1 timeout_expect $timeout "$command" || ret=$?
else
		$command || ret=$?
fi

exit $ret
