
#The configuration variables that are used during the program
export PYTHON_EXEC='/usr/bin/python3'
export MCL_SHELL='0'

#Various folder and files locations
LOCAL_DIR="@@LOCAL_DIR@@"
ROOT_DIR="/www/codacom/private/codacom-0.1"
export CONVERT_DIR=$ROOT_DIR'/src/conversion'
export TOOLS_DIR=$ROOT_DIR'/src/tools'
export TMP_DIR=$ROOT_DIR'/tmp'
export METHOD_DIR=$ROOT_DIR'/methods'
export PREPROCESSING_DIR=$ROOT_DIR'/src/preprocessing'
export ANALYSIS_DIR=$ROOT_DIR'/src/analysis'
export OUTPUT_DIR=$LOCAL_DIR'/clusterings'
export INPUT_DIR=$LOCAL_DIR'/preprocessed/inputs'
export RAW_INPUT_DIR=$LOCAL_DIR'/raw/inputs'
export RAW_GROUND_TRUTH_DIR=$LOCAL_DIR'/raw/ground_truth'
export TRANSLATION_DIR=$LOCAL_DIR'/preprocessed/translation'
export RESULTS_DIR=$LOCAL_DIR'/results'
export GROUND_TRUTH_DIR=$LOCAL_DIR'/preprocessed/ground_truth'
export LOG_FILE=$LOCAL_DIR'/log'

#The accepted input graph formats
export FORMATS=".txt .csv .gml .graphml"

#Behaviour configuration
export METHOD_TIMEOUT=1000 #the timeout of community detection methods (in s)
export PARALLEL=0 #Should the methods be run in parallel?
export NB_PROCESS=2
export VERBOSE=1 #Light verbose
export VERBOSE_FULL=0 #High-density verbose

#Execution subparts
export A_CORRELATION="0"
export A_QUALITY="0"
export A_FULL_REPORT="0"
export C_VISUALISATION="0"
export C_TEX="0"
export C_QUOTIENT="0"

#Whitelists
export WL_RAW=""
export WL_INPUT="football.txt"
export WL_METHOD="label_prop conclude"
export WL_QF="modularity local_modularity modularity_density clust_coef s_compactness"
export WL_CM="fbcubed nmi omega_index"

#Due to a lack of dependancies, some methods may be ignored
export EXCEPT_METHOD=' '

#Change the root dir of the dir variables (simple appen)
root() {   
    export CONVERT_DIR=$1/$CONVERT_DIR
    export OUTPUT_DIR=$1/$OUTPUT_DIR
    export INPUT_DIR=$1/$INPUT_DIR
    export METHOD_DIR=$1/$METHOD_DIR
    export ANALYSIS_DIR=$1/$ANALYSIS_DIR
    export RESULTS_DIR=$1/$RESULTS_DIR
    export TMP_DIR=$1/$TMP_DIR
    export LOG_FILE=$1/$LOG_FILE
}

#Print functions
err() {
    printf "["$(date +"%T")"]" >> $LOG_FILE
    echo "Error : "$@ >> $LOG_FILE
    >&2 echo "Error : "$@
    exit 1
}

verb() {
    printf "["$(date +"%T")"]" >> $LOG_FILE
    if [ $VERBOSE != 0 ]
    then
	echo $@ | tee -a $LOG_FILE
    else
	echo $@ >> $LOG_FILE
    fi
}

verb_f() {
    printf "["$(date +"%T")"]" >> $LOG_FILE
    if [ $VERBOSE_FULL != 0 ]
    then
	echo $@ | tee -a $LOG_FILE
    else
	echo $@ >> $LOG_FILE
    fi
}


lock() {
    #Check locks
    touch $TMP_DIR/tmp
    LOCKED=1
    while [ $LOCKED != 0 ]
    do
	lock=0
	while [ $LOCKED != 0 ] && [ $lock -lt $NB_PROCESS ]
	do
	    lock=$(( lock + 1 ))
	    if ln -s $TMP_DIR/tmp $TMP_DIR/$lock 2> /dev/null
	    then
		verb_f "Creating parallel lock #$lock"
		LOCKED=0
	    fi
	done
	if [ $LOCKED != 0 ]
	then
	    sleep 2
	fi
    done
}

unlock() {
    TODEL=1
    i=0
    while [ $TODEL != 0 ] && [ $i -lt $NB_PROCESS ]
    do
	i=$(( i + 1 ))
	if rm $TMP_DIR/$i 2> /dev/null
	then
	    verb_f "Removing parallel lock #$i"
	    TODEL=0
	fi
    done
}
