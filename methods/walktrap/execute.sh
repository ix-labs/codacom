set -e
DESCRIPTION="The walktrap algorithm, from Pons&Latapy, 2005"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a walktrap
