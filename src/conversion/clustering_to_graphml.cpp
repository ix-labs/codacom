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
#include <cstring>
#include <libgen.h>
#include <cstdlib>

#include <config.h>

# if WRITE_GRAPHML_TWO_ARGS
#   define igraph_write_graph_graphml(a,b,c) igraph_write_graph_graphml(a,b)
# endif

# if !HAVE_IGRAPH_CATTRIBUTE_HAS_ATTR
#   if HAVE_IGRAPH_I_ATTRIBUTE_HAS_ATTR
#      define igraph_cattribute_has_attr(a,b,c) igraph_i_attribute_has_attr(a,b,c)
#   elif HAVE_HAS_ATTR
#      define igraph_cattribute_has_attr(a,b,c) has_attr(a,b,c)
#   endif
# endif

#include "file_operations.hpp"

void usage(const char *more) {
  std::cerr << more << std::endl;
  std::cerr << "usage: ./clustering_to_graphml [-g] graph clust_dir prefix outfile" << std::endl << std::endl;
  std::cerr << "Converts a set of clusterings of the same graph (they are in the c_dir and have a name prefixed by prefix) to a single graphml file with edge and nodes labels that are representatives o" << std::endl;
  std::cerr << "-g\tChange the input format to graphml" << std::endl;
  std::cerr << "graph\tInput graph file (edgelist)" << std::endl;
  std::cerr << "clust_dir\tThe directory where clusterings are for this graph" << std::endl;
  std::cerr << "prefix\tThe prefix which the clusterings will have" << std::endl;
  std::cerr << "outfile\tOutput graph file (graphml)" << std::endl;
  exit(1);
}


int main(int argc, char* argv[]) {
  bool in_graphml = 0;
  if(argc < 3)
    usage("Not enough arguments");
  if(argc > 3)
    if(std::string(argv[1]) == "-g")
      in_graphml = 1;
  std::string s_infile = argv[1 + in_graphml];
  std::string s_indir = argv[2 + in_graphml];
  std::string s_prefix = argv[3 + in_graphml];
  std::string s_outfile = argv[4 + in_graphml];

  igraph_t graph;
  igraph_i_set_attribute_table(&igraph_cattribute_table);
  FILE* in = fopen(s_infile.c_str(), "r");
  if(in_graphml)
    igraph_read_graph_graphml(&graph, in, 0);
  else
    igraph_read_graph_edgelist(&graph, in, 0, false);
  fclose(in);

  std::list<std::string> files;
  list_files(s_indir, s_prefix, files);

  char temp[1000];
  for(std::list<std::string>::iterator iter = files.begin(); iter != files.end(); iter++) {
    std::vector<long> memb;
    read_membership(*iter, memb);
    strcpy(temp, (*iter).c_str());
    std::string filebase = basename(temp);
    std::string method = filebase.substr(s_prefix.length());

    igraph_es_t es = igraph_ess_all(IGRAPH_EDGEORDER_ID);
    igraph_eit_t eit;
    igraph_eit_create(&graph, es, &eit);
    while(!IGRAPH_EIT_END(eit)) {
      igraph_integer_t eid = IGRAPH_EIT_GET(eit), from, to, from_id, to_id;
      igraph_edge(&graph, eid, &from, &to);
      from_id = from;
      to_id = to;
      if(igraph_cattribute_has_attr(&graph, IGRAPH_ATTRIBUTE_VERTEX, "id_comm")) {
	from_id = VAN(&graph, "id_comm", from);
	to_id = VAN(&graph, "id_comm", to);
      }
      std::string s = "membership_"+method;
      SETVAN(&graph, s.c_str(), from, memb[from_id]);
      SETVAN(&graph, s.c_str(), to, memb[to_id]);
      if(memb[from_id] == memb[to_id])
	SETEAN(&graph, ("intercommunity_"+method).c_str(), eid, 0);
      else
	SETEAN(&graph, ("intercommunity_"+method).c_str(), eid, 1);
      IGRAPH_EIT_NEXT(eit);
    }
  }

  FILE* out = fopen(s_outfile.c_str(), "w");
  igraph_write_graph_graphml(&graph, out, false);
  fclose(out);
}
