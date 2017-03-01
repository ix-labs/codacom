#ifndef ALGORITHMS
#define ALGORITHMS

#include <igraph/igraph.h>

void clauset(igraph_t* graph, igraph_vector_t* membership);
void spinglass(igraph_t* graph, igraph_vector_t* membership);
void leading_eigenvector(igraph_t* graph, igraph_vector_t* membership);
void walktrap(igraph_t* graph, igraph_vector_t* membership);
void betweenness(igraph_t* graph, igraph_vector_t* membership);
void label_prop(igraph_t* graph, igraph_vector_t* membership);
void infomap(igraph_t* graph, igraph_vector_t* membership);
#endif
