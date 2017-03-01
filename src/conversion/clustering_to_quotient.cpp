// Copyright GREYC - UMR 6072 ; Université de Caen Normandie
// Esplanade de la paix
// CS 14032
// 14032 Caen CEDEX 5
// contributeur :
// Jean Creusefond, 2015
// 
// jean.creusefond@unicaen.fr
// 
// Ce logiciel est un programme informatique servant à la comparaison et à l'analyse d'outils de détection de communauté.
// 
// Ce logiciel est régi par la licence CeCILL-B soumise au droit français et respectant les principes de diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou redistribuer ce programme sous les conditions de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA sur le site "http://www.cecill.info".
// 
// En contrepartie de l'accessibilité au code source et des droits de copie, de modification et de redistribution accordés par cette licence, il n'est offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons, seule une responsabilité restreinte pèse sur l'auteur du programme, le titulaire des droits patrimoniaux et les concédants successifs.
// 
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement,  à l'utilisation,  à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels avertis possédant  des  connaissances  informatiques approfondies.  Les utilisateurs sont donc invités à charger  et  tester  l'adéquation  du logiciel à leurs besoins dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
// 
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL-B, et que vous en avez accepté les termes.
// 
// ==========================================================================
// 
// Copyright GREYC - UMR 6072 ; Université de Caen Normandie
// Esplanade de la paix
// CS 14032
// 14032 Caen CEDEX 5
// contributor(s) :
// Jean Creusefond, 2015
// 
// jean.creusefond@unicaen.fr
// 
// This software is a computer program whose purpose is to compare and analyse community detection tools.
// 
// This software is governed by the CeCILL-B license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-B
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// 
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability.
// 
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or
// data to be ensured and,  more generally, to use and operate it in the
// same conditions as regards security.
// 
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL-B license and that you accept its terms. 
#include <igraph/igraph.h>
#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>

#include <map>

#include <config.h>

# if WRITE_GRAPHML_TWO_ARGS
#   define igraph_write_graph_graphml(a,b,c) igraph_write_graph_graphml(a,b)
# endif

//Compatibility with igraph 0.5
#include <config.h>

#if GET_EID_FIVE_ARGS
#   define igraph_get_eid(a,b,c,d,e,f) if(f){igraph_get_eid(a,b,c,d,e);}else{igraph_get_eid2(a,b,c,d,e);}
#endif

#include "file_operations.hpp"

void usage(const char *more) {
  std::cerr << more << std::endl;
  std::cerr << "usage: ./clustering_to_quotient graph clustering outfile" << std::endl << std::endl;
  std::cerr << "Converts the input graph clustering to a quotient graph" << std::endl;
  std::cerr << "infile\tInput graph file (edgelist)" << std::endl;
  std::cerr << "clustering\tInput clustering file" << std::endl;
  std::cerr << "outfile\tOutput graph file (graphml)" << std::endl;
  std::cerr << "options :" << std::endl;
  exit(1);
}

void usage(std::string s) {
  usage(s.c_str());
}

int main(int argc, char* argv[]) {
  if(argc < 3)
    usage("Not enough arguments");

  opterr = 0;
  char c;
  while ((c = getopt(argc, argv, "h")) != -1) {
    switch(c) {
    case 'h':
      usage("Program aborting, help asked");
      break;
    case '?':
      if (optopt == 'i' || optopt == 'o')
				usage("Option -" + std::string(1, optopt) + " requires an argument");
      else if (isprint (optopt))
				usage("Unknown option -" + std::string(1, optopt));
      else
				usage("Unknown option character " + std::string(1, optopt));
    default:
      usage("Unsupported error");
    }
  }

  std::string s_infile = argv[1];
  std::string s_clustering = argv[2];
  std::string s_outfile = argv[3];

  igraph_t graph;
  igraph_i_set_attribute_table(&igraph_cattribute_table);
  FILE* in = fopen(s_infile.c_str(), "r");
  int error;
  error = igraph_read_graph_edgelist(&graph, in, 0, false);
  fclose(in);

  if(error) {
    std::cerr << "Can't open the input file " << s_infile << std::endl;
    exit(1);
  }
  
  //Getting the map clust_id -> vector of nodes in the cluster
  std::vector<long> memb;
  read_membership(s_clustering, memb);
  std::map<long, std::vector<long> > clust_to_nodes;
  for(unsigned long v = 0; v < memb.size(); v++) {
    long id = memb[v];
    std::map<long, std::vector<long> >::iterator nodes = clust_to_nodes.find(memb[v]);
    if(nodes == clust_to_nodes.end()) { //If the key was not found
      std::vector<long> *vect = new std::vector<long>;
      clust_to_nodes[id] = *vect;
    }
    SETVAN(&graph, "id_comm", v, memb[v]);
    clust_to_nodes[id].push_back(v);
  }
  
  igraph_t g_q; //Quotient graph
  igraph_empty(&g_q, clust_to_nodes.size(), 0);
  long i = 0;
  //Creating the map community id -> node id in quotient graph
  std::map<long, long> com_to_node;
  std::map<long, std::vector<long> >::iterator iter;
  for(iter = clust_to_nodes.begin(); iter != clust_to_nodes.end(); iter++) {
    long q_id = iter->first;
    com_to_node[q_id] = i;
    SETVAN(&g_q, "id_comm", i, q_id);
    i++;
  }
  
  //Creating the quotient graph with weights
  for(iter = clust_to_nodes.begin(); iter != clust_to_nodes.end(); iter++) {
    long community = iter->first;
    long volume = 0;
    std::vector<long>::iterator iter2;
    for(iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++) { //Going over all nodes of the cluster
      long v = *iter2;
      igraph_vector_t neis;
      igraph_vector_init(&neis, 0);
      igraph_neighbors(&graph, &neis, v, IGRAPH_ALL);
      volume += igraph_vector_size(&neis);

      for(long k = 0; k < igraph_vector_size(&neis); k++) {
				long u = VECTOR(neis)[k];
				if(u > v) { //We only consider the nodes of a higher id to not consider the same edge twice
					long community2 = VAN(&graph, "id_comm", u);
					if(community2 != community) { //If this is an intercommunity edge
						long q_v = com_to_node[community];
						long q_u = com_to_node[community2];
						igraph_integer_t eid;
						igraph_get_eid(&g_q, &eid, q_v, q_u, 0, 0);
						if(eid == -1) { //If no edge exists between these communities
							igraph_add_edge(&g_q, q_v, q_u);
							igraph_get_eid(&g_q, &eid, q_v, q_u, 0, 0);
							SETEAN(&g_q, "weight_com", eid, 1);
						} else {
							SETEAN(&g_q, "weight_com", eid, EAN(&g_q, "weight_com", eid) + 1);
						}
					}
				}
      }
      
      SETVAN(&g_q, "volume_com", com_to_node[iter->first], volume);
      SETVAN(&g_q, "size_com", com_to_node[iter->first], iter->second.size());
    }
  }
  
  FILE* out = fopen(s_outfile.c_str(), "w");
  error = igraph_write_graph_graphml(&g_q, out, false);
  fclose(out);
  
  if(error) {
    std::cerr << "Can't open the output file " << s_outfile << std::endl;
    exit(1);
  }
  
  igraph_destroy(&graph);
}
