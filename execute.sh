#!/bin/sh

set -e

usage() {
    echo $1
    echo "Usage : execute.sh [options]* [vars_file]"
    echo "options : "
    echo "-p\tOnly do the preprocessing"
    echo "-a\tOnly do the analysis"
    echo "-c\tOnly do the conversion of the results"
    echo "-m\tOnly execute the methods"
    echo "vars_file\tPath to the vars.sh file"
}

ANALYSIS=0
CONVERSION=0
METHODS=0
PREPROCESSING=0
SUBSET=1 #Is only a subset of the program to be executed?

while getopts ":acmp" opt; do
    case $opt in
	a)
	    SUBSET=0
	    ANALYSIS=1
	    ;;
	c)
       	    SUBSET=0
	    CONVERSION=1
	    ;;
	m)
	    SUBSET=0
	    METHODS=1
	    ;;
	p)
	    SUBSET=0
	    PREPROCESSING=1
	    ;;
	h)
	    usage
	    exit 0
	    ;;
	\?)
	    usage "Invalid option: -$OPTARG"
	    exit 1
	    ;;
	:)
	    usage "The option -$OPTARG requires an argument"
	    exit 1
	    ;;
    esac
done

if [ $SUBSET != 0 ]
then
    ANALYSIS=1
    CONVERSION=1
    METHODS=1
    PREPROCESSING=1
fi

shift $((OPTIND-1)) #Shift the input arguments to remove the processed options

if [ $# -ge 1 ]
then
    VARS=$1
else
    VARS=vars.sh
fi

if [ ! "$(expr substr $VARS 1 1)" = "/" ] #If the first character is not a /, it is a relative path
then
    VARS=$(pwd)/$VARS
fi
. $VARS

rm -f $TMP_DIR/*
rm -f $LOG_FILE
verb "Starting Community Detection Algorithm Comparator (CoDACom) the "$(date +"%m-%d-%y")" at "$(date +"%T")


RAW_INPUTS=""
for f in $RAW_INPUT_DIR/*
do
    if [ -f $f ]
    then
	base=$(basename $f)
	if echo " "$WL_RAW" "|grep -q " "$base" "
	then
	    RAW_INPUTS=$f" "$RAW_INPUTS
	else
	    verb_f "Potential raw input "$f" will be ignored (not in whitelist)"
	fi
    fi
done
verb_f "The raw input graphs : "$RAW_INPUTS


INPUTS=""
for f in $INPUT_DIR/*.txt
do
    if [ -f $f ]
    then
	base=$(basename $f)
	if echo " "$WL_INPUT" "|grep -q " "$base" "
	then
	    INPUTS=$f" "$INPUTS
	else
	    verb_f "Potential input "$f" will be ignored (not in whitelist)"
	fi
    fi
done
verb_f "The input graphs : "$INPUTS

#List of the methods to run
ALL_METHODS=""
for d in $METHOD_DIR/*/execute.sh
do
    if [ -e $d ]
    then
	r="[^/?\*\:;{}]"
	m=$(echo $d | sed "s:$METHOD_DIR/\($r$r*\)/execute.sh:\1:") #Get the method name
	if echo " "$WL_METHOD" "|grep -q " "$m" "
	then
	    ALL_METHODS=$ALL_METHODS" "$m
	else
	    verb_f "Potential method "$m" will be ignored (not in whitelist)" 
	fi
    fi
done
verb_f "Methods that will be applied : "$ALL_METHODS

##PREPROCESSING
if [ $PREPROCESSING != 0 ]
then
    verb "Starting preprocessing"
    for f in $RAW_INPUTS
    do
	if [ -f $f ]
	then
	    verb_f "Preprocessing file "$f
	    base=$(basename $f)
	    base="${base%.*}" #Remove the extension
	    if [ -e $RAW_GROUND_TRUTH_DIR/$base ]
	    then
		verb_f "Ground truth "$RAW_GROUND_TRUTH_DIR/$base" found, it will be preprocessed as well"
		$PREPROCESSING_DIR/simplify -b -t $TRANSLATION_DIR/$base".trans" $f $INPUT_DIR/$base".txt" $RAW_GROUND_TRUTH_DIR/$base $GROUND_TRUTH_DIR/$base
	    else
		$PREPROCESSING_DIR/simplify -b -t $TRANSLATION_DIR/$base".trans" $f $INPUT_DIR/$base".txt"
	    fi
	fi
    done
fi

##METHODS EXECUTION
if [ $METHODS != 0 ]
then
    verb "Starting methods"
    rm -f $RESULTS_DIR/time/raw_times #rm the previous times if those existed
    cd $METHOD_DIR
    sh execute.sh $VARS #Executes the methods
    cd - > /dev/null
fi
wait #Wait for the parallel jobs to finish

##ANALYSIS
if [ $ANALYSIS != 0 ]
then
    verb "Starting analysis"
    for f in $INPUTS
    do
	base=$(basename -s .txt $f)
	if [ $A_CORRELATION != 0 ]
	then
	    verb_f "Analysing the graph "$base" (correlation)"
	    OPTIONS=""
	    for cm in $WL_CM
	    do
		OPTIONS=$OPTIONS" -c "$cm
	    done
	    $ANALYSIS_DIR/algs_correlation -p "$base"_ $OPTIONS $OUTPUT_DIR $RESULTS_DIR/correlation_$base 
	fi
	if [ $A_QUALITY != 0 ]
	then
	    OPTIONS=""
	    for qf in $WL_QF
	    do
		OPTIONS=$OPTIONS" -q "$qf
	    done

	    if [ -f $GROUND_TRUTH_DIR/$base ]
	    then
		OPTIONS=$OPTIONS" -t "$GROUND_TRUTH_DIR/$base
		for cm in $WL_CM
		do
		    OPTIONS=$OPTIONS" -c "$cm
		done
	    fi
	    verb_f "Analysing the graph "$base" (quality)"
	    if [ $PARALLEL != 0 ]
	    then
		lock
		$ANALYSIS_DIR/algs_quality -g $f -i $OUTPUT_DIR -o $RESULTS_DIR/quality/quality_$base -p "$base"_ $OPTIONS && unlock &
	    else
		$ANALYSIS_DIR/algs_quality -g $f -i $OUTPUT_DIR -o $RESULTS_DIR/quality/quality_$base -p "$base"_ $OPTIONS
	    fi
	    
	fi
	if [ $A_FULL_REPORT != 0 ]
	then
	    base=$(basename -s .txt $f)
	    for c in "$OUTPUT_DIR/$base"_*
	    do
		if [ -f $c ]
		then
		    verb_f "Creating the full report of the file $c"
		    basec=$(basename $c)
		    outdir=$RESULTS_DIR/full_reports/$basec
		    mkdir -p $outdir 
		    options=" "
		    for q in $WL_QF
		    do
			options=$options"-q "$q" "
		    done
		    $ANALYSIS_DIR/alg_full_report$options$f $c $outdir
		fi
	    done
	fi
    done
    
    wait

    #Converting the quality files to rank files
    for f in $RESULTS_DIR/quality/quality_*
    do
	if [ -f $f ]
	then
	    base=$(basename $f)
	    outfile=$RESULTS_DIR/quality/rank_$base
	    verb_f "Reading quality file "$f" and converting it to ranks in "$outfile
	    $PYTHON_EXEC $ANALYSIS_DIR/quality_to_rank.py $f $outfile
	fi
    done

    ranks_files=""
    for f in $RESULTS_DIR/quality/rank_*
    do
	if [ -f $f ]
	then
	    ranks_files=$ranks_files"$f "
	fi
    done
		
		if [ "$ranks_files" != "" ]
		then
				$PYTHON_EXEC $ANALYSIS_DIR/collect_spearman.py $RESULTS_DIR/quality/ranks_ $ranks_files
		fi

    #Ranking of the algorithms
    nb=$(echo "$INPUTS" | wc -w)
    nb2=$(echo "$METHODS" | wc -w)
    if [ $nb -gt 1 ] && [ $nb2 -gt 1 ]
    then
	#Taking the mean value of ranks and correlations
	verb_f "Taking the mean/stderr of the quality ranks "$RESULTS_DIR/quality/rank_quality_mean
	$PYTHON_EXEC $ANALYSIS_DIR/mean_results.py $RESULTS_DIR/quality rank_quality $RESULTS_DIR/quality/mean_rank_quality
	verb_f "Taking the mean/stderr of the correlation "$RESULTS_DIR/correlation_mean
	$PYTHON_EXEC $ANALYSIS_DIR/mean_results.py $RESULTS_DIR/ correlation $RESULTS_DIR/mean_correlation
    fi

    #Transforming the raw time to a table
    if [ -e $RESULTS_DIR/time/raw_times ]
    then
	$PYTHON_EXEC $ANALYSIS_DIR/raw_time_to_table.py $RESULTS_DIR/time/raw_times $RESULTS_DIR/time/table_times
    fi    
fi

#CONVERSION OF DATA
if [ $CONVERSION != 0 ]
then
    verb "Starting conversion"
    #Creating a graphml file to visualise the clustering (if a graphml exists in the input dir, it will be used as a basis)
    if [ $C_VISUALISATION != 0 ]
    then
	for g in $INPUTS
	do
	    baseg=$(basename -s .txt $g)
	    options=""
	    if [ -f $INPUT_DIR/$baseg".graphml" ] #If there is a graphml, we use it as a basis
	    then
		verb_f "Base graphml found for graph "$baseg
		options=$options" -g"
		g=$INPUT_DIR/$baseg".graphml"
	    fi
	    
	    verb_f "Converting the clusterings of "$baseg" to graphml "$RESULTS_DIR/visualisation/$baseg.graphml
	    $CONVERT_DIR/clustering_to_graphml $options $g $OUTPUT_DIR $baseg"_" $RESULTS_DIR/visualisation/$baseg.graphml
	done
    fi

    #Creating tex tables from results
    if [ $C_TEX != 0 ]
    then
	for f in $RESULTS_DIR/quality/*
	do
	    base=$(basename $f)
	    verb_f "Converting the results "$f" to a latex tabular "$RESULTS_DIR/tex/$base
	    $PYTHON_EXEC $CONVERT_DIR/data_to_tex.py $f $RESULTS_DIR/tex/$base
	done
	for f in $RESULTS_DIR/*
	do
	    if [ -f $f ]
	    then
		base=$(basename $f)
		verb_f "Converting the results "$f" to a latex tabular "$RESULTS_DIR/tex/$base
		$PYTHON_EXEC $CONVERT_DIR/data_to_tex.py $f $RESULTS_DIR/tex/$base
	    fi
	done
    fi

    #Creating a quotient graph for each clustering
    if [ $C_QUOTIENT != 0 ]
    then
	for g in $INPUTS
	do
	    baseg=$(basename -s .txt $g)
            for f in "$OUTPUT_DIR/$baseg"_*
            do
		base=$(basename $f)
		verb_f "Converting the clustering "$f" to quotient graph "$RESULTS_DIR/quotient/$base.graphml
		$CONVERT_DIR/clustering_to_quotient $g $f $RESULTS_DIR/quotient/$base.graphml
            done	
	done
    fi
fi

verb "Ending Community Detection Algorithm Comparator (CoDACom) the "$(date +"%m-%d-%y")" at "$(date +"%T")
