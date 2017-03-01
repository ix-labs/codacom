set -e
DESCRIPTION="The Markov CLustering algorithm, from Von Dongen, 'Graph clustering by flow simulation', 2000"
. ../template.sh

if [ $MCL_SHELL != "" ]
then
    base=$(basename -s .txt $INPUT_GRAPH)
    $MCL_SHELL $INPUT_GRAPH --abc -V all -o $TMP_DIR/$base\_mcl 2> /dev/null
    $PYTHON_EXEC $TOOLS_DIR/communities_to_membership.py $TMP_DIR/$base\_mcl $OUTPUT_DIR/$base\_mcl
    rm $TMP_DIR/$base\_mcl
fi
