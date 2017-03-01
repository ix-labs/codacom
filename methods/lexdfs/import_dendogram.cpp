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

  infile.open(Parameters::getDendFile(), std::ifstream::in);
  std::string line;
  std::vector<std::pair<long, long> > merges;
  while(getline(infile, line)) {
    long i, j;
    sscanf(line.c_str(), "%li %li", &i, &j);
    merges.push_back(std::make_pair(i, j));
  }

  g[boost::graph_bundle].m = num_edges(g);
  g[boost::graph_bundle].n = num_vertices(g);
  long max_id = g[boost::graph_bundle].n;
  //std::cout << "n : " << g[boost::graph_bundle].n << " m : " << g[boost::graph_bundle].m << std::endl;
  
  //Keeps tracks of which community id is which
  std::map<long, Community*> id_to_community;
  std::vector<Community*> map_vertex_community(g[boost::graph_bundle].n);
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    Community* c = new Community(*vp.first, g);
    init_experiments_create(*c);
    map_vertex_community[*vp.first] = c;
    id_to_community[*vp.first] = c;
  }
  init_experiments();

  for(long i = 0; i < merges.size(); i++) {
    Community *c1, *c2;
    long id1 = merges[i].first, id2 = merges[i].second;
    c1 = id_to_community[id1];
    c2 = id_to_community[id2];
    if(c1->size < c2->size) {
      Community *temp = c1;
      c1 = c2;
      c2 = temp;
    }
    pre_merged_community_experiments(*c1, *c2, map_vertex_community);
    c1->merge(*c2, map_vertex_community);
    merged_community_experiments(*c1, map_vertex_community);
    id_to_community[max_id] = c1;
    max_id++;
  }
}
