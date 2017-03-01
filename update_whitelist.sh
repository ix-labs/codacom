set -e

print_list() {
		varname=$1
		shift 1
		if [ $USE_ARGS != 0 ]
		then
				list=$ARGS
		else
				list=$@
		fi

		if [ $TO_LIST = 0 ]
		then
				sh $TOOLS_DIR/change_var.sh $VARS $varname $list
		else
				for e in $list
				do
						echo $e
				done
		fi
}

usage() {
    echo $1
    echo "Usage : execute.sh [options]* [whitelisted]*"
		echo "Updates the whitelists that are in the vars.sh file with the directory content"
		echo "whitelisted : item to put in the whitelist if the -d option is selected"
    echo "options : "
		echo "-l\tDoes not update, but prints the list of possible items in the standard output."
    echo "-r\tUpdates the raw inputs whitelist"
    echo "-i\tUpdates the inputs whitelist"
    echo "-m\tUpdates the methods whitelist"
    echo "-q\tUpdates the quality functions whitelist"
    echo "-c\tUpdates the comparison methods whitelist"
    echo "-d\tChanges the whitelist to the whitelisted items instead of the content"
    echo "-h\tPrints this help and exits"
		echo "-v file\tThe vars.sh file"
		echo "NOTE : the -l and -d option are mutually exclusive."
		echo "Moreover, these two options require to pick only one list to update/print"
		exit 1
}

RAW=0
INPUTS=0
METHODS=0
QUALITY=0
COMPARISON=0
TO_LIST=0
USE_ARGS=0
SUBSET=1 #Is only a subset of the program to be executed?
ARGS=""
VARS=./vars.sh

while getopts ":ldrimqchv:" opt; do
    case $opt in
				l)
						TO_LIST=1
						;;
				d)
						USE_ARGS=1
						;;
				r)
						SUBSET=0
						RAW=1
						;;
				i)
						SUBSET=0
						INPUTS=1
						;;
				m)
						SUBSET=0
						METHODS=1
						;;
				q)
						SUBSET=0
						QUALITY=1
						;;
				c)
						SUBSET=0
						COMPARISON=1
						;;
				h)
						usage
						exit 0
						;;
				v)
						VARS=$OPTARG
						;;
				\?)
						usage "Invalid option: -$OPTARG"
						;;
				:)
						usage "The option -$OPTARG requires an argument"
						;;
    esac
done

shift $((OPTIND-1))
ARGS="$@"

if [ $SUBSET != 0 ]
then
		RAW=1
    INPUTS=1
    METHODS=1
    QUALITY=1
    COMPARISON=1
fi

if [ $USE_ARGS != 0 ] && [ $TO_LIST != 0 ]
then
		usage "Args and listing action are mutually exclusive"
fi

if [ $USE_ARGS != 0 ] || [ $TO_LIST != 0 ]
then
		if [ $(($RAW + $INPUTS + $METHODS + $QUALITY + $COMPARISON)) -gt 1 ]
		then
				usage "No more than one whitelist can be selected if the list behaviour is select or if whitelist elements are provided"
		fi
fi

if [ ! "$(expr substr $VARS 1 1)" = "/" ] #If the first character is not a /, it is a relative path
then
		VARS=$(pwd)/$VARS
fi
. $VARS

if [ $RAW != 0 ]
then
    WL_RAW=""
    request=""
    for p in $FORMATS
    do
				request=$request" $RAW_INPUT_DIR/*$p"
    done

		for f in $request
    do
				if test -e $f
				then
						base=$(basename $f)
    				WL_RAW=$base" "$WL_RAW
				fi
    done
    print_list "WL_RAW" $WL_RAW
fi


if [ $INPUTS != 0 ]
then
    WL_INPUT=""
    for d in $INPUT_DIR/*.txt
    do
				if test -e $d
				then
            i=$(basename $d)
    				WL_INPUT=$i" "$WL_INPUT
				fi
    done
    print_list "WL_INPUT" $WL_INPUT
fi

if [ $METHODS != 0 ]
then
    WL_METHOD=""
    for d in $METHOD_DIR/*/execute.sh
    do
				if [ -e $d ]
				then
						r="[^/?\*\:;{}]"
						m=$(echo $d | sed 's:'"$METHOD_DIR"'/\('$r$r'*\)/execute.sh:\1:') #Get the method name
						if ! echo " "$EXCEPT_METHOD" "|grep -q " "$m" "
						then
								WL_METHOD=$m" "$WL_METHOD
						else
								echo "Due to the configuration of exceptions, method $m is unavailable. This behaviour appears by default because of a lack of build dependancies." 1>&2
						fi
				fi
    done
		print_list "WL_METHOD" $WL_METHOD
fi

if [ $QUALITY != 0 ]
then
    WL_QF=""
    if [ -e $TOOLS_DIR/list_qf ]
    then
				WL_QF=$($TOOLS_DIR/list_qf)
				print_list "WL_QF" $WL_QF
    else
				echo "$TOOLS_DIR/list_qf not found, have you built your distribution?"
    fi
fi


if [ $COMPARISON != 0 ]
then
    WL_CM=""
    if [ -e $TOOLS_DIR/list_cm ]
    then
				WL_CM=$($TOOLS_DIR/list_cm)
				print_list "WL_CM" $WL_CM
    else
				echo "$TOOLS_DIR/list_cm not found, have you built your distribution?"
    fi
fi
