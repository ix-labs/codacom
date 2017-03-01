set -e
DESCRIPTION="The infomap algorithm from Rosvall&Bergstrom, 'Maps of random walks on complex networks reveal community structure', 2008"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a infomap
