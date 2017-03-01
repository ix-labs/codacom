set -e
DESCRIPTION="The spinglass algorithm, from Reichardt&Bornholdt, 'Statistical mechanics of community detection', 2006"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a spinglass
