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
//#include <boost/lambda/lambda.hpp>
#include <iostream>
// #include <iterator>
// #include <algorithm>
#include<fstream>
//#include <boost/graph/graphml.hpp>
//#include <boost/graph/graphviz.hpp>
#include <boost/config.hpp>
#include <boost/version.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/static_assert.hpp>
#include <boost/graph/property_maps/null_property_map.hpp> 
#include <boost/graph/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/graph/betweenness_centrality.hpp>

using namespace boost;

#include "search/Graph.hpp"
#include "search/LexDFS.hpp"
#include "search/CommunityDetection.hpp"
#include "search/Parameters.hpp"
#include "search/CommonIntervalDetection.hpp"
#include "search/EdgeWeightDetection.hpp"

void mark_connected_components(Graph& g, long edge_iteration_threshold) {
  std::list<vertex_t> vertices;
  std::map<vertex_t, std::list<vertex_t>::iterator> v_to_p;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    vertex_t v = *vp.first;
    vertices.push_back(v);
    v_to_p[v] = --vertices.end();
    g[v].tmp1 = -1;
  }

  long i = 0;
  while(!vertices.empty()) {
    std::deque<vertex_t> q;
    q.push_back(vertices.back());
    vertices.pop_back();
    
    while(!q.empty()) {
      vertex_t v = q.back();
      q.pop_back();
      g[v].tmp1 = i;
      
      typedef boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;
      std::pair<adjacency_iterator, adjacency_iterator> it;
      it = boost::adjacent_vertices(v, g);
      for( ; it.first != it.second; ++it.first) {
	vertex_t u = *it.first;
	edge_t e = boost::edge(u,v,g).first;
	if(g[u].tmp1 < 0 && g[e].iteration <= edge_iteration_threshold) {
	  q.push_back(u);
	  g[u].tmp1 = i;
	  vertices.erase(v_to_p[u]);
	}
      }
    }
    i++;
  }
}

long triangles = 0;
double clust(vertex_t v, Graph& g) {
  std::vector<vertex_t> neis;
  typedef boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;
  std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(v, g);
  for( ; it.first != it.second; ++it.first) {
    vertex_t neigh = *(it.first);
    neis.push_back(neigh);
  }
  long tr = 0;
  for (long i = 0; i < neis.size(); i++) {
    for(long j = i+1; j < neis.size(); j++) {
      if(boost::edge(neis[i], neis[j], g).second) {
	tr++;
      }
    }
  }
  triangles += tr;
  if(neis.size() > 1)
    return ((double)(tr*2))/(neis.size()*(neis.size() - 1));
  else 
    return 0.0;
}


/*
  Command-line arguments (must be written in this order):
  -model (c: cascade, t:threshold)
  -options (options specific to the model, namely the probability of the cascade and the threshold)
  -start (r: random, integer_id:the id of the starting node)
 */
int main(int argc, char* argv[])
{
  Parameters::analyse_args(argc, argv);

  /*
    Read file
   */
  std::ifstream infile;
  infile.open(Parameters::getInputGraphFile(), std::ifstream::in);
  Graph g;
  //Reads edgelist file
  read_edgelist(infile, g);
  infile.close();

  g[boost::graph_bundle].m = num_edges(g);
  g[boost::graph_bundle].n = num_vertices(g);
  //std::cout << "n : " << g[boost::graph_bundle].n << " m : " << g[boost::graph_bundle].m << std::endl;

  /*
    Apply processing
   */
  //Finds a random node
  boost::minstd_rand gen(posix_time::microsec_clock::local_time().time_of_day().total_milliseconds());   
  lexdfs(g, boost::random_vertex(g, gen), true);

  if(Parameters::getAnalyser() == EdgeWeight)
    compute_edge_order_difference(g, 1);

  for(int i=1; i<=Parameters::getNbIters(); i++) {
    lexdfs(g, boost::random_vertex(g, gen), Parameters::getMethod() == LexDFS);
    if(Parameters::getAnalyser() == EdgeWeight) {
      compute_edge_order_difference(g, i+1);
    }
  }

  long imax = 0;
  if(Parameters::getAnalyser() == CommonIntervals)
    common_intervals_clustering(g);
  if(Parameters::getAnalyser() == EdgeWeight)
    imax = lexdfs_clustering(g, true, Parameters::getQfToMax());

  mark_connected_components(g, imax);

  //Should we output the membership?
  double sum_clust = 0;
  if(false) {
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
      std::cout << *vp.first << " " << g[*vp.first].tmp1 << std::endl;
    }
  }

  //Prints lexDFS score and betweenness centrality for every edge
  // std::map<vertex_t, float> tmp1;
  // boost::associative_property_map<std::map<vertex_t, float> > v_betw_map(tmp1);
  // std::map<edge_t, float> tmp2;
  // boost::associative_property_map<std::map<edge_t, float> > e_betw_map(tmp2);
  // brandes_betweenness_centrality(g, v_betw_map, e_betw_map);

  // typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
  // std::pair<edge_iter, edge_iter> iter;
  // for (iter = edges(g); iter.first != iter.second; ++iter.first)
  //   std::cout << g[*(iter.first)].lexDFS_score << " " << get(e_betw_map, *(iter.first)) << std::endl;

  // typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
  // std::pair<vertex_iter, vertex_iter> vp;
  // for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
  //   std::cout << g[*vp.first].visited << std::endl;    
  // }

  /*
    Write file
   */
  // std::ofstream outfile;
  // outfile.open(Parameters::getOutputGraphFile());

  // write_graphml(outfile, g, dp, true);
  // outfile.close();

  return EXIT_SUCCESS;
}
