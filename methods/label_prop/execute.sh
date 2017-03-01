set -e
DESCRIPTION="The label propagation algorithm, from Raghavan et al., 'Near linear time algorithm to detect community structures in large-scale networks', 2007"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a label_prop
