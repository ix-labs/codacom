set -e
DESCRIPTION="Clauset's greedy modularity algorithm, from Clauset et al., 'Finding community structure in very large networks', 2004"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a clauset
