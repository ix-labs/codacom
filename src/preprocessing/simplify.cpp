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
#include <iomanip>
#include <string.h>
#include <cstdlib>

#include <config.h>
#if SIMPLIFY_THREE_ARGS
#   define igraph_simplify(a,b,c,d) igraph_simplify(a,b,c)
#endif
#if IGRAPH_BFS_SIX_ARGS
#   define igraph_bfs(a,b,c,d,e,f,g,h,i,j,k,l,m,n) igraph_bfs(a,b,d,g,NULL,i)
#endif

#include "file_operations.hpp"

void usage(const char *more) {
  std::cerr << more << std::endl;
  std::cerr << "usage: ./simplify [-b][-n][-t translation] infile outfile [clustering out_clustering]" << std::endl << std::endl;
  std::cerr << "Removes the double edges and self loops of the input file in the edgelist format. It also selects the largest connected component." << std::endl;
  std::cerr << "-b\tRuns a bfs and reorders the nodes with the visit number" << std::endl;
  std::cerr << "-t\tOutput a 'translation file' making the correspondance between old and new ids" << std::endl;
  std::cerr << "infile\tInput graph file (edgelist)" << std::endl;
  std::cerr << "outfile\tOutput graph file (edgelist)" << std::endl;
  std::cerr << "clustering\tA clustering (often a ground-truth) that needs to be updated as well (written in output)" << std::endl;
  std::cerr << "Note : if a clustering/ground-truth has been given, the program will remove all the nodes not appearing in the clustering and then remove the nodes that are not the largest connected component" << std::endl;
  exit(1);
}

/**
 * Only keeps the largest component of the graph
 * Warning : the ids are not kept
 * @param[in,out] graph The graph to consider
 */
void remove_small_components(igraph_t* graph) {
  igraph_vector_t membership, csize;
  igraph_vector_init(&membership, 0);
  igraph_vector_init(&csize, 0);
  igraph_integer_t number;
  igraph_clusters(graph, &membership, &csize, &number, IGRAPH_WEAK);
  long max = 0;
  long imax = -1;
  for(long i = 0; i < igraph_vector_size(&csize); i++) {
    if(igraph_vector_e(&csize, i) > max) {
      max = igraph_vector_e(&csize, i);
      imax = i;
    }
  }

  igraph_vector_t not_wcc;
  igraph_vector_init(&not_wcc, 0);
  igraph_vit_t vit;
  igraph_vit_create(graph, igraph_vss_all(), &vit);
  while (!IGRAPH_VIT_END(vit)) {
    long id = IGRAPH_VIT_GET(vit);
    if(igraph_vector_e(&membership, id) != imax) {
      igraph_vector_push_back(&not_wcc, id);
    }
    IGRAPH_VIT_NEXT(vit);
  }
  igraph_vit_destroy(&vit);

  igraph_delete_vertices(graph, igraph_vss_vector(&not_wcc));
}

/**
 * Imports a graph from a file (the import method depends on the extension)
 * @param[out] graph unitialised graph
 * @param[in] filename path to the file
 */
void import_graph(igraph_t* graph, std::string filename) {
  FILE* in = fopen(filename.c_str(), "r");
  std::string input_format = "";
  int error;

  std::size_t found = filename.rfind(".");
  if(found != std::string::npos)
    input_format = filename.substr(filename.rfind('.') + 1);

  if(input_format == "gml")
    error = igraph_read_graph_gml(graph, in);
  else if(input_format == "graphml")
    error = igraph_read_graph_graphml(graph, in, 0);
	else if(input_format == "ncol")
    error = igraph_read_graph_ncol(graph, in, NULL, false, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
  else if(input_format == "lgl")
    error = igraph_read_graph_lgl(graph, in, false, IGRAPH_ADD_WEIGHTS_NO, IGRAPH_UNDIRECTED);
  else if(input_format.size() > 0)
    error = igraph_read_graph_edgelist(graph, in, 0, false);

  if(error) {
    std::cerr << "Problem when parsing the input file " << filename << ". Error code : " << error << std::endl;
    exit(1);
  }

  fclose(in);  
}

int main(int argc, char* argv[]) {
  bool bfs = false;
  bool translation = false;
  std::string s_translation;
  std::map<long, std::list<long> > membership; //Membership in ground truth
  igraph_vector_t bfs_rank;
  igraph_vector_init(&bfs_rank, 0);

  int shift = 0;

  if(argc > 1) {
    if(strcmp(argv[1], "-b") == 0) {
      bfs = true;
      argc--;
      shift += 1;
    }
    if(argc > 1 && strcmp(argv[1+shift], "-t") == 0) {
      if(argc < 2)
				usage("No translation file given as output");
      
      translation = true;
      s_translation = argv[2+shift];
      argc -= 2;
      shift += 2;
    }
  }
  if(argc < 3)
    usage("Not enough arguments");
  
  igraph_t graph;
  igraph_i_set_attribute_table(&igraph_cattribute_table);
  import_graph(&graph, argv[1+shift]);

  igraph_vit_t vit;
  igraph_vit_create(&graph, igraph_vss_all(), &vit);
  while (!IGRAPH_VIT_END(vit)) {
    long id = IGRAPH_VIT_GET(vit);
    SETVAN(&graph, "old_id", id, id);
    IGRAPH_VIT_NEXT(vit);
  }
  igraph_vit_destroy(&vit);
  igraph_simplify(&graph, 1, 1, NULL);
 
  //If there is a ground-truth, we need to :
  //-Remove every vertice that is not in this ground-truth
  //-Remove all the vertices that are not connected to the LCC anymore
  //-Output the clustering with the updated ids
  if(argc > 3) {
    if(argc < 5)
      usage("You need to specify an output file for the clustering");
    std::string s_clust_in = argv[3+shift];

    //Read the clustering into a map
    read_membership(s_clust_in, membership);
    igraph_vector_t not_gt;
    igraph_vector_init(&not_gt, 0);
    
    //Looping over all vertices
    igraph_vit_create(&graph, igraph_vss_all(), &vit);
    while (!IGRAPH_VIT_END(vit)) {
      long old_id = VAN(&graph, "old_id", IGRAPH_VIT_GET(vit));
      if(membership.find(old_id) == membership.end())
				igraph_vector_push_back(&not_gt, IGRAPH_VIT_GET(vit));
      IGRAPH_VIT_NEXT(vit);
    }
    igraph_vit_destroy(&vit);
    //Deleting all vertices not in any ground-truth
    igraph_delete_vertices(&graph, igraph_vss_vector(&not_gt));
  }

  //Keeping only the largest CC
  remove_small_components(&graph);
  if(bfs) {
    srand(time(NULL));
    igraph_integer_t v = rand()%((int)igraph_vcount(&graph));
    igraph_vector_t temp;
    igraph_vector_init(&temp, 0);
		//For backward compatibility, we compute the ranks ourself
    igraph_bfs(&graph, v, NULL, IGRAPH_ALL, 0, NULL,
							 &temp, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if(igraph_vector_size(&temp) != igraph_vcount(&graph)) {
      std::cerr << "Error : " << igraph_vector_size(&temp) << " vertices were visited by the bfs while there are " << igraph_vcount(&graph) << std::endl;
      exit(1);
    }
		igraph_vector_resize(&bfs_rank, igraph_vcount(&graph));
    std::cout << "After simplifying, file " << argv[1+shift] << " n : " << igraph_vcount(&graph) << ", m : " << igraph_ecount(&graph) << std::endl;
    for(long i =0; i<igraph_vector_size(&temp); i++) //Changing vids to rank
			igraph_vector_set(&bfs_rank, igraph_vector_e(&temp, i), i);
		igraph_vector_destroy(&temp);
  }
  
  if(argc > 3) {
    if(argc < 5)
      usage("You need to specify an output file for the clustering");
    std::string s_clust_out = argv[4+shift];
    
    //Output updated clustering in file
    std::ofstream outfile;
    outfile.open(s_clust_out.c_str());
    outfile << std::fixed << std::setprecision(0);
    igraph_vit_create(&graph, igraph_vss_all(), &vit);
    while (!IGRAPH_VIT_END(vit)) {
      long id = IGRAPH_VIT_GET(vit);
      long old_id = VAN(&graph, "old_id", id);
      
      if(bfs) 
				outfile << igraph_vector_e(&bfs_rank, IGRAPH_VIT_GET(vit)); 
      else
				outfile << id;
      for(std::list<long>::iterator iter = membership[old_id].begin(); iter != membership[old_id].end(); iter++)
				outfile << " " << *iter;
      outfile << std::endl;
      IGRAPH_VIT_NEXT(vit);
    }
    igraph_vit_destroy(&vit);
  }

  //Translation file
  if(translation) {
    std::ofstream out_trans;
    out_trans.open(s_translation.c_str());
    out_trans << std::fixed << std::setprecision(0);
    igraph_vit_create(&graph, igraph_vss_all(), &vit);
    while (!IGRAPH_VIT_END(vit)) {
      igraph_integer_t id = IGRAPH_VIT_GET(vit);
      igraph_integer_t old_id = VAN(&graph, "old_id", id);
      if(bfs)
				id = igraph_vector_e(&bfs_rank, id);

      out_trans << old_id << " " << id << std::endl;;
      IGRAPH_VIT_NEXT(vit);
    }
    igraph_vit_destroy(&vit);
  }

  std::ofstream outfile;
  outfile.open(argv[2+shift]);
  outfile << std::fixed << std::setprecision(0);

  igraph_eit_t eit;
  igraph_eit_create(&graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);
  while (!IGRAPH_EIT_END(eit)) {
    long eid = IGRAPH_EIT_GET(eit);
    igraph_integer_t from, to;
    igraph_edge(&graph, eid, &from, &to);
    if(bfs)
      outfile << igraph_vector_e(&bfs_rank, from) << " " << igraph_vector_e(&bfs_rank, to) << std::endl;
    else
      outfile << from << " " << to << std::endl;
    IGRAPH_EIT_NEXT(eit);
  } 
  
	// FILE* out = fopen(argv[2+shift], "w");
	// igraph_write_graph_edgelist(&graph, out);
	// fclose(out);

}
