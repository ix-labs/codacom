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
#include "CommunityDetection.hpp"
#include "EdgeWeightDetection.hpp"

typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;

bool comp_for_ranks(std::pair<edge_t, double> left, std::pair<edge_t, double> right) {
  return left.second < right.second;
}

void compute_lexdfs_ranks(Graph& g, std::map<edge_t, double>& ranks) {
  std::vector<std::pair<edge_t, double> > data;
  edge_iter ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    data.push_back(std::make_pair(*ei, g[*ei].lexDFS_score));
  }
  sort(data.begin(), data.end(), comp_for_ranks);
  long i = 0;
  while(i < data.size()) {
    long k = i;
    while(k+1 < data.size() && data[i].second == data[k+1].second) {
      k++;
    }
    for(long j = i; j <= k; j++) {
      ranks[data[j].first] = i + ((double)(k-i))/2;
    }
    i = k+1;
  }
}

double spearman(std::map<edge_t, double>& r1, std::map<edge_t, double>& r2) {
  double sum = 0;
  long n = r1.size();
  for(std::map<edge_t, double>::iterator it = r1.begin(); it != r1.end(); it++) {
    edge_t e = it->first;
    double diff = r1[e] - r2[e];
    sum += 6*diff*diff/(n*n - 1);
  }
  return 1 - sum/n;
}

void compute_edge_order_difference(Graph& g, long nb_iter) {
  bool print_convergence = true;
  vertex_t v1, v2;
  edge_iter ei, ei_end;
  long n = g[boost::graph_bundle].n;

  std::map<edge_t, double> r1, r2;
  if(print_convergence) {
    compute_lexdfs_ranks(g, r1);
  }
    
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    v1 = boost::source(*ei, g);
    v2 = boost::target(*ei, g);
    //std::cout << g[v1].visited << " " << g[v2].visited << " " << (double)abs(g[v1].visited - g[v2].visited) << std::endl;
    double new_score = g[*ei].lexDFS_score*(((double)(nb_iter-1.0))/nb_iter);
    new_score += (1 - ((double)abs(g[v1].visited - g[v2].visited))/n)/nb_iter;
    g[*ei].lexDFS_score = new_score;
  }

  if(print_convergence) {
    compute_lexdfs_ranks(g, r2);
    std::cout << spearman(r1, r2) << std::endl;
  }
}

bool compare_edges(const EdgeProperties* first, const EdgeProperties* second) {
  return first->lexDFS_score > second->lexDFS_score;
}

long lexdfs_clustering(Graph& g, bool max /*=false*/, QualityFunction q/*=Modularity*/) {
  //Inits the community list
  std::vector<Community*> map_vertex_community(g[boost::graph_bundle].n);
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    Community* c = new Community(*vp.first, g);
    init_experiments_create(*c);
    map_vertex_community[*vp.first] = c;
  }
  //Inits the edge list
  std::list<EdgeProperties*> edge_list;
  edge_iter ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    edge_list.push_back(&(g[*ei]));
    g[*ei].descriptor = *ei;
  }
  edge_list.sort(compare_edges); //Sorts in decreasing order
  init_experiments(max, q);
  //Start of the real deal
  std::list<EdgeProperties*>::iterator iter = edge_list.begin();
  double vmax = 0;
  long imax = 0;
  long i = 1;
  while(iter != edge_list.end()) {
    vertex_t v1 = boost::source((*iter)->descriptor, g);
    vertex_t v2 = boost::target((*iter)->descriptor, g);
    
    Community* c1 = map_vertex_community[v1];
    Community* c2 = map_vertex_community[v2];

    if(c1 != c2) {
      if(c1->size < c2->size) {
	Community* tmp = c1;
	c1 = c2;
	c2 = tmp;
      }
      pre_merged_community_experiments(*c1, *c2, map_vertex_community);
      c1->merge(*c2, map_vertex_community);
      double value = merged_community_experiments(*c1, map_vertex_community);
      if (max && value > vmax) {
	vmax = value;
	imax = i;
      }
      delete c2;
      i++;
    }
    iter++;
  }
  return imax;
}
