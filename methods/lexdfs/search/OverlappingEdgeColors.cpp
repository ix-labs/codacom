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
#include "OverlappingEdgeColors.hpp"
#include <iostream>
#include <fstream>

#define UPPER_BOUND 100000

typedef boost::graph_traits<Graph>::edge_iterator edge_iter;

EdgeCommunities::EdgeCommunities(Graph& _g) : g(_g) {
  communities.resize(g[boost::graph_bundle].m);
}

long domain(Graph& g) {
  //return g[boost::graph_bundle].m;
  return 10;
}

struct Constraint {
  long id_from;
  long id_to;
  bool equal;
};

void write_constraints(std::string filepath, long m, long domain, double cost, std::list<Constraint> constraints) {
  std::ofstream outfile;
  outfile.open(filepath);

  outfile << "test " << m << " " << domain << " " << constraints.size() << " " << UPPER_BOUND << std::endl;
  for(long i = 0; i<m; i++)
    outfile << domain << " ";
  outfile << std::endl << std::endl;

  for(std::list<Constraint>::iterator it = constraints.begin(); it != constraints.end(); it++) {
    double default_cost, exception_cost;
    if(it->equal) {
      default_cost = 1;
      exception_cost = 0;
    } else {
      default_cost = 0;
      exception_cost = cost;
    }
    outfile << "2 " << it->id_from << " " << it->id_to << " " << default_cost << " " << domain << std::endl;
    for(long i =0; i<domain; i++)
      outfile << i << " " << i << " " << exception_cost << std::endl;
    outfile << std::endl;
  }
  outfile.close();
}

void read_solution(std::string filepath, std::vector<long>& communities) {
  std::ifstream infile;
  infile.open(filepath);

  long edge_id = 0;
  long com_val;
  while(infile >> com_val) {
    communities[edge_id] = com_val;
    edge_id++;
  }
  infile.close();
}

void EdgeCommunities::execute(std::string input_file, std::string output_file, std::string execute_command, double cost) {
  std::vector<bool> treated;
  treated.resize(g[boost::graph_bundle].m);

  std::list<Constraint> constraints;
  
  std::pair<edge_iter, edge_iter> iter;
  for (iter = edges(g); iter.first != iter.second; ++iter.first)
    treated[g[*(iter.first)].id] = false;

  for (iter = edges(g); iter.first != iter.second; ++iter.first) {
    long e1_id = g[*(iter.first)].id;
    treated[e1_id] = true;

    vertex_t u1 = boost::source(*(iter.first),g);
    vertex_t v1 = boost::target(*(iter.first), g);
    //std::cout << "edge id : " << e1_id << " u label : " << g[u1].label << " v label : " << g[v1].label << std::endl;

    typename boost::graph_traits<Graph>::out_edge_iterator out_i, out_end;

    //Loop over the out edges of both ends
    for(int i = 0; i<2; i++) {
      for (tie(out_i, out_end) = out_edges(u1, g); out_i != out_end; ++out_i) {
	edge_t e2 = *out_i;
	long e2_id = g[e2].id;
	if(!treated[e2_id]) {
	  Constraint c;
	  c.id_from = std::min(e1_id, e2_id);
	  c.id_to = std::max(e1_id, e2_id);

	  vertex_t u2 = boost::source(e2, g);
	  vertex_t v2 = boost::target(e2, g);
	  if(u2 == u1)
	    u2 = v2;

	  if((edge(v1, u2, g)).second) { //If an edge was found, there is a triangle
	    c.equal = true;
	  } else {
	    c.equal = false;
	  }
	  //std::cout << c.id_from << " " << c.id_to << " " << c.equal << std::endl;
	  constraints.push_back(c);
	}
      }
      vertex_t tmp = u1;
      u1 = v1;
      v1 = tmp;
    }
  }
  std::cout << "Finish constraints" << std::endl;
  write_constraints(input_file, g[boost::graph_bundle].m, domain(g), cost, constraints);
  std::cout << "Finish write" << std::endl;
  read_solution(output_file, communities);
}

void EdgeCommunities::apply() {
  std::pair<edge_iter, edge_iter> iter;
  for (iter = edges(g); iter.first != iter.second; ++iter.first) {
    long edge_id = g[*(iter.first)].id;
    g[*(iter.first)].community = communities[edge_id];
  }

}

//Obsolete : first draft
// void Coloring::update_edge(edge_t e, std::vector<std::vector<double> >& map) {
//   vertex_t u1 = boost::source(e,g);
//   vertex_t v1 = boost::target(e,g);
//   long edge_id = g[e].id;
  
//   std::vector<double> delta;
//   for(int i = 0; i<d; i++)
//     delta.push_back(0.0);
  
//   typename boost::graph_traits<Graph>::out_edge_iterator out_i, out_end;

//   for (tie(out_i, out_end) = out_edges(u1, g); out_i != out_end; ++out_i) {
//     edge_t e2 = *out_i;
//     vertex_t u2 = boost::source(e2, g);
//     vertex_t v2 = boost::target(e2, g);
//     if(u2 == u1)
//       u2 = v2;

//     if((edge(v1, u2, g)).second) { //If an edge was found, there is a triangle
//       for(int i = 0; i<d; i++)
// 	delta[i] += (colors[g[e2].id][i] - colors[edge_id][i])/2;
//     } else {
//       for(int i = 0; i<d; i++)
// 	delta[i] += (-colors[g[e2].id][i] + colors[edge_id][i])/2;
//     }
//   }

//   for (tie(out_i, out_end) = out_edges(v1, g); out_i != out_end; ++out_i) {
//     edge_t e2 = *out_i;
//     vertex_t u2 = boost::source(e2, g);
//     vertex_t v2 = boost::target(e2, g);
//     if(u2 == v1)
//       u2 = v2;

//     if((edge(v1, u2, g)).second) { //If an edge was found, there is a triangle
//       for(int i = 0; i<d; i++)
// 	delta[i] += (colors[g[e2].id][i] - colors[edge_id][i])/2;
//     } else {
//       for(int i = 0; i<d; i++)
// 	delta[i] += (-colors[g[e2].id][i] + colors[edge_id][i])/2;
//     }
//   }

//   for(int i = 0; i<d; i++)
//     map[edge_id].push_back(0.0);
//   for(int i = 0; i<d; i++) {
//     map[edge_id][i] = colors[edge_id][i] + delta[i]/(g[u1].degree + g[v1].degree - 2);
//     if(map[edge_id][i] > 1)
//       map[edge_id][i] = 1;
//     if(map[edge_id][i] < 0)
//       map[edge_id][i] = 0;
//   }
// }
