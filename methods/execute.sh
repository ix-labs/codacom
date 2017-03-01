#!/bin/sh

set -e

usage() {
    echo $1
    echo "Usage : execute.sh [options]* vars"
    echo "Executes the methods in the subdirs on the input files localised in the vars shell file."
    echo "vars : a shell file containing the system-dependant variables (position of directories etc.)"
}

if [ $# -ge 1 ]
then
    VARS=$1
else
    usage "Not enough arguments ($@), at least 2 required"
    exit 1
fi

shift 1
. $VARS

gather_time() {
    out_file=$1 #The file on which to save the result
    pref=$2 #A prefix to put before the time
    shift 2
    cmd=$@ #The command to execute

    # Wrapped by the time command that outputs the result in stderr
    # Therefore the stderr&stdout of the command are put in the stdout and both outputs are then switched
    # The whole thing therefore outputs in stdout the times returned by time, and both stdout and stderr of the original command are redirected to stderr
    ret=0
    #times=$(time -p ($cmd 2>&1) 3>&1 1>&2 2>&3) || ret=$?
    if command -v time > /dev/null #Check wether the time command is available
		then
				times=$( (time -p $cmd) 3>&1 1>&2 2>&3) || ret=$?
				if [ $ret = 0 ]
				then
						t=$(echo "${times}" | grep "user" | sed "s:user\ \(\w*\):\1:") #Retrieve the user time
						echo "$pref $t" >> $out_file
				fi
		else
				$cmd 2&>1
		fi
		return $ret
}

execute_method() {
    dir=$1
    INPUT_GRAPH=$2
		
    verb_f "Executing method $dir on input $INPUT_GRAPH"
    ret=0
    base=$(basename -s .txt $INPUT_GRAPH)
    gather_time $RESULTS_DIR/time/raw_times "$dir $base" "/bin/sh timeout.sh $METHOD_TIMEOUT $dir /bin/sh execute.sh $INPUT_GRAPH $VARS" || ret=$?
    #timeout "execute.sh $ROOT_DIR $INPUT_GRAPH $VARS" $METHOD_TIMEOUT || ret=$?
    if [ $ret = 0 ]
    then
				verb_f "Successful end of the method "$dir" on file "$INPUT_GRAPH
    else
				rm -f $OUTPUT_DIR/$(basename -s .txt $INPUT_GRAPH)_$dir #Removing the output
				if [ $ret = 15 ]
				then
    				verb "Timeout after ${METHOD_TIMEOUT} seconds of the method "$dir" on file "$INPUT_GRAPH
				else
            verb "Crash of the method "$dir" on file "$INPUT_GRAPH", exit status : "$ret
						exit $ret;
				fi
    fi
    verb_f "Ending of method $dir on input $INPUT_GRAPH"
		
    if [ $PARALLEL != 0 ]
    then
				unlock
    fi
}

for f in $WL_INPUT
do
    INPUT_GRAPH=$INPUT_DIR/$f
    if [ -f $INPUT_GRAPH ]
    then
	verb "Applying methods on file "$f
	for m in $WL_METHOD #Loop over the input methods
	do
	    if [ $m = "all" ]
	    then
		for i in *
		do 
		    if [ -e $i/execute.sh ]
		    then
			if [ $PARALLEL != 0 ]
			then
			    lock
			    execute_method $i $INPUT_GRAPH &
			else
			    execute_method $i $INPUT_GRAPH
			fi
		    fi
		done
	    else
		if [ -d $m ]
		then
		    if [ $PARALLEL != 0 ]
		    then
			lock
			execute_method $m $INPUT_GRAPH &
		    else
			execute_method $m $INPUT_GRAPH
		    fi
		else
		    err "Directory "$m" not found, exiting program"
		fi
	    fi
	done
    else
	err "File $INPUT_GRAPH not found, exiting program"
    fi
done

wait
