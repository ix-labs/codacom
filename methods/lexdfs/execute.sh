set -e
DESCRIPTION="The LexDFS-based algorithm (maximum modularity state is outputted), from Creusefond et al., 'Finding compact communities in large graphs', 2015"
. ../template.sh

./lexdfs -input $INPUT_GRAPH -exp global modularity > $OUTPUT_CLUST
