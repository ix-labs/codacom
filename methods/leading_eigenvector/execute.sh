set -e
DESCRIPTION="The leading eigenvector method, from Newman, 'Finding community structure in networks using the eigenvectors of matrices', 2006"
. ../template.sh

../igraph/algs -i $INPUT_GRAPH -o $OUTPUT_DIR -a leading_eigenvector
