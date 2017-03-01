set -e
DESCRIPTION="Betweenness centrality method, from Girvan&Newman, 'Finding and evaluating community structure in networks', 2004"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a betweenness
