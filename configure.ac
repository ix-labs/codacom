AC_PREREQ([2.69])
# Prelude.
AC_INIT([CoDACom],[0.1.1], [jean.creusefond@unicaen.fr])
AC_CONFIG_SRCDIR([src/analysis/algs_quality.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR([AC_MSG_ERROR([This package needs ar])])
LT_INIT
AM_PATH_PYTHON([3.0],,[AC_MSG_ERROR([This package needs python, version at least >= 3])])
AC_CHECK_PROG([EXPECT], [expect], 0, 1)
if test $EXPECT != 0
then
	AC_MSG_WARN([This package can use the expect command to put a timer on executions])
fi

#Check for programs specific to some methods
AC_CHECK_PROGS([MCL], [mcl], [0])
if test "$MCL" = 0
then
	AC_MSG_WARN([MCL program was not found. This method will be ignored])
fi

LEXDFS=1
BOOST=1
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], [], [BOOST=0])

if test "$BOOST" = 0
then
	LEXDFS=0
	AC_MSG_WARN([Boost header missing. LexDFS method will be ignored])
fi
AC_LANG_POP([C++])

# Checks for libraries.
AC_CHECK_LIB([igraph], [igraph_empty], [], [AC_MSG_ERROR([This package needs igraph])])

# Checks for header files.
AC_CHECK_HEADERS([float.h malloc.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AX_FUNC_WRITE_GRAPHML_TWO_ARGS()
AX_FUNC_LEAD_EIG_ELEVEN_ARGS()
AX_FUNC_GET_EID_FIVE_ARGS()
AX_FUNC_SIMPLIFY_THREE_ARGS()
AX_FUNC_IGRAPH_BFS_SIX_ARGS()
AC_CHECK_FUNCS(igraph_cattribute_has_attr igraph_i_attribute_has_attr has_attr)
HAVE_IGRAPH_COMMUNITY_INFOMAP=0
AC_CHECK_FUNCS(igraph_community_infomap, HAVE_IGRAPH_COMMUNITY_INFOMAP=1, [AC_MSG_ERROR([Your igraph version is too old. Please upgrade to a version >= 0.6.])])

AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor pow sqrt strtol])

# Output files.
AC_CONFIG_HEADERS([config.h])


#Structure variables
PREPROCESSING_DIR=$(pwd)/src/preprocessing
ANALYSIS_DIR=$(pwd)/src/analysis
CONVERT_DIR=$(pwd)/src/conversion
OUTPUT_DIR=$(pwd)/clusterings
METHOD_DIR=$(pwd)/methods
RESULTS_DIR=$(pwd)/results
TMP_DIR=$(pwd)/tmp
INPUT_DIR=$(pwd)/preprocessed/inputs
GROUND_TRUTH_DIR=$(pwd)/preprocessed/ground_truth
TRANSLATION_DIR=$(pwd)/preprocessed/translation
RAW_INPUT_DIR=$(pwd)/raw/inputs
RAW_GROUND_TRUTH_DIR=$(pwd)/raw/ground_truth
TOOLS_DIR=$(pwd)/src/tools
LOG_FILE=$(pwd)/log

#Test of the existence of source files for louvain & conclude
LOUVAIN=1
if test ! -f $METHOD_DIR/louvain/community.cpp
then
    AC_MSG_WARN([Louvain method source code not found])
    LOUVAIN=0
fi
CONCLUDE=1
if test ! -f $METHOD_DIR/conclude/CONCLUDE.jar
then
    AC_MSG_WARN([Conclude jar not found])
    CONCLUDE=0
fi

AC_SUBST(CONVERT_DIR, $CONVERT_DIR)
AC_SUBST(OUTPUT_DIR, $OUTPUT_DIR)
AC_SUBST(INPUT_DIR, $INPUT_DIR)
AC_SUBST(RAW_INPUT_DIR, $RAW_INPUT_DIR)
AC_SUBST(METHOD_DIR, $METHOD_DIR)
AC_SUBST(ANALYSIS_DIR, $ANALYSIS_DIR)
AC_SUBST(PREPROCESSING_DIR, $PREPROCESSING_DIR)
AC_SUBST(RESULTS_DIR, $RESULTS_DIR)
AC_SUBST(TMP_DIR, $TMP_DIR)
AC_SUBST(GROUND_TRUTH_DIR, $GROUND_TRUTH_DIR)
AC_SUBST(RAW_GROUND_TRUTH_DIR, $RAW_GROUND_TRUTH_DIR)
AC_SUBST(TRANSLATION_DIR, $TRANSLATION_DIR)
AC_SUBST(TOOLS_DIR, $TOOLS_DIR)
AC_SUBST(LOG_FILE, $LOG_FILE)
AC_SUBST(EXPECT, $EXPECT)

#Populating whitelists
EXCEPTIONS=" "
if test "$LEXDFS" = 0
then
    EXCEPTIONS=$EXCEPTIONS"lexdfs "
fi
if test "$MCL" = 0
then
    EXCEPTIONS=$EXCEPTIONS"mcl "
fi
if test "$HAVE_IGRAPH_COMMUNITY_INFOMAP" = 0
then
    EXCEPTIONS=$EXCEPTIONS"infomap "
fi
if test "$LOUVAIN" = 0
then
    EXCEPTIONS=$EXCEPTIONS"louvain "
fi
if test "$CONCLUDE" = 0
then
    EXCEPTIONS=$EXCEPTIONS"conclude "
fi
AC_SUBST(EXCEPT_METHOD, $EXCEPTIONS)

WL_METHOD=""
for d in $METHOD_DIR/*/execute.sh
do
				if test -e $d
				then
								r="[[^/?\*\:;{}]]"
								#Get the method name (use --posix on sed if you want to check that the regex is posix)
								m=$(echo $d | sed 's:'"$METHOD_DIR"'/\('$r$r'*\)/execute.sh:\1:') 
								tmp=" $m "
								if test "${EXCEPTIONS#*$tmp}" = "$EXCEPTIONS" #If the method is not a substring of the exceptions, we do not add it
								then
												WL_METHOD=$m" "$WL_METHOD
								fi
				fi
done
AC_SUBST(WL_METHOD, $WL_METHOD)

WL_INPUT=""
for d in $INPUT_DIR/*.txt
do
    if test -e $d
    then
        i=$(basename $d)
    	WL_INPUT=$i" "$WL_INPUT
    fi
done
AC_SUBST(WL_INPUT, $WL_INPUT)

#Creating files (Makefiles, scripts, directories)
AM_CONDITIONAL([BOOST], test "$BOOST" = "1")
AM_CONDITIONAL([LOUVAIN], test "$LOUVAIN" = "1")
#AM_CONDITIONAL([LOUVAIN], false)
AC_CONFIG_FILES([Makefile src/libmisc/Makefile src/libcomparison/Makefile src/libquality/Makefile src/tools/Makefile src/analysis/Makefile src/conversion/Makefile src/preprocessing/Makefile methods/Makefile methods/igraph/Makefile methods/louvain/Makefile methods/k-core/Makefile methods/lexdfs/Makefile vars.sh:vars.in])

mkdir -p $INPUT_DIR
mkdir -p $RAW_INPUT_DIR
mkdir -p $TRANSLATION_DIR
mkdir -p $OUTPUT_DIR
mkdir -p $TMP_DIR
mkdir -p $GROUND_TRUTH_DIR
mkdir -p $RAW_GROUND_TRUTH_DIR
mkdir -p $RESULTS_DIR
mkdir -p $RESULTS_DIR/quality
mkdir -p $RESULTS_DIR/visualisation
mkdir -p $RESULTS_DIR/quotient
mkdir -p $RESULTS_DIR/tex
mkdir -p $RESULTS_DIR/time
mkdir -p $RESULTS_DIR/full_reports

AC_OUTPUT
