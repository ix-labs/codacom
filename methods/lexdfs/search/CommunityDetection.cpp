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
#include <iostream>
#include "Parameters.hpp"
#include <queue>
#include <stack>
#include <algorithm>

/*
  Careful : this class is NOT thread-safe
*/

Community::Community(Graph& graph) : vertices(), size(1), nb_edges(0), g(graph) {
};

Community::Community(vertex_t v, Graph& graph) : Community(graph){
  vertices.push_back(v);
};

void Community::merge(Community& c, std::vector<Community*>& map_vertex_community) {
  while(!c.vertices.empty()) {
    vertex_t v = c.vertices.front();
    c.vertices.pop_front();
    map_vertex_community[v] = this;
    vertices.push_front(v);
    size++;
  }
}

long Community::volume() {
  long volume = 0;
  for(std::list<vertex_t>::iterator iter = vertices.begin(); iter!=vertices.end(); iter++)
    volume += degree(*iter, g);
  return volume;
}

long Community::nb_inter_edges(Community& c, std::vector<Community*>& map_vertex_community) {
  long inter_edges = 0;

  for(std::list<vertex_t>::iterator iter = c.vertices.begin(); iter!=c.vertices.end(); iter++) {
    std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(*iter, g);
    for( ; it.first != it.second; ++it.first) {
      vertex_t neigh = *(it.first);
      if(map_vertex_community[neigh] == this) { 
	inter_edges++; 
      }
    }
  }
  return inter_edges;
}

/*Part reserved to the experiments*/
typedef boost::graph_traits<Graph> GTraits;
typename GTraits::out_edge_iterator ei, ei_end;
long diameter(Community& c, std::vector<Community*>& map_vertex_community) {
  std::queue<vertex_t> queue;
  long d = 0;
  
  //Approximate computation
  long r = rand() % c.vertices.size();
  //Finding random node
  long i = 0;
  std::list<vertex_t>::iterator iter = c.vertices.begin();
  while(i < r) {
    i++;
    iter++;
  }
  vertex_t v1 = *iter;
  vertex_t v2 = v1;
  
  //Finding the node the most far away from v1
  for(std::list<vertex_t>::iterator iter2 = c.vertices.begin(); iter2!=c.vertices.end(); iter2++)
    c.g[*iter2].tmp1 = -1;
  
  c.g[v1].tmp1 = 0;
  queue.push(v1);
  
  while (!queue.empty()) {
    vertex_t u = queue.front();
    queue.pop();
    for (boost::tie(ei, ei_end) = out_edges(u, c.g); ei != ei_end; ++ei) {
      vertex_t v = target(*ei, c.g);
      if (map_vertex_community[v]==&c && c.g[v].tmp1==-1) {
	queue.push(v);
	c.g[v].tmp1 = c.g[u].tmp1 + 1;
	if(c.g[v].tmp1 > d) {
	  d = c.g[v].tmp1;
	  v2 = v;
	}
      }
    }
  }

  //Finding the excentricity of v2
  for(std::list<vertex_t>::iterator iter2 = c.vertices.begin(); iter2!=c.vertices.end(); iter2++)
    c.g[*iter2].tmp1 = -1;
  
  c.g[v2].tmp1 = 0;
  queue.push(v2);
  
  while (!queue.empty()) {
    vertex_t u = queue.front();
    queue.pop();
    for (boost::tie(ei, ei_end) = out_edges(u, c.g); ei != ei_end; ++ei) {
      vertex_t v = target(*ei, c.g);
      if (map_vertex_community[v]==&c && c.g[v].tmp1==-1) {
	queue.push(v);
	c.g[v].tmp1 = c.g[u].tmp1 + 1;
	if(c.g[v].tmp1 > d) {
	  d = c.g[v].tmp1;
	}
      }
    }
  }
  
  //Exact computation
  /*
    for(std::list<vertex_t>::iterator iter = c.vertices.begin(); iter!=c.vertices.end(); iter++) {
    //tmp1 : distance from origin initialized as -1 (grey/black : tmp2>-1)
    //Change the tmp1 value of all the vertices inside the community to -1 (white)
    for(std::list<vertex_t>::iterator iter2 = c.vertices.begin(); iter2!=c.vertices.end(); iter2++)
    c.g[*iter2].tmp1 = -1;
    
    c.g[*iter].tmp1 = 0;
    queue.push(*iter);

    while (!queue.empty()) {
    vertex_t u = queue.front();
    queue.pop();
    for (boost::tie(ei, ei_end) = out_edges(u, c.g); ei != ei_end; ++ei) {
    vertex_t v = target(*ei, c.g);
    if (map_vertex_community[v]==&c && c.g[v].tmp1==-1) {
    queue.push(v);
    c.g[v].tmp1 = c.g[u].tmp1 + 1;
    if(c.g[v].tmp1 > d)
    d = c.g[v].tmp1;
    }
    }
    }
    }
  */
  return d;
}

double compactness(Community& c) {
  if(c.diameter > 0)
    return ((double)c.nb_edges)/c.diameter;
  return 0;
}

double modularity(Community& c) {
  long m = c.g[boost::graph_bundle].m;
  double ai = ((double)c.vol)/(2*m);
  double mod = ((double)c.nb_edges)/m - ai*ai;
  return mod;
}

double local_conductance(Community& c) {
  long ext_edges = c.vol - c.nb_edges*2;
  long min = fmin(c.vol, 2*c.g[boost::graph_bundle].m - c.vol);
  double cond = 1;
  if(min != 0)
    cond = ((double)ext_edges)/min;
  return cond;
}

double conductance(Community& c) {
  return ((1 - local_conductance(c))*c.size)/c.g[boost::graph_bundle].n;
}

double local_clust_coef(Community& c) {
  return c.avg_clust;
}

double clust_coef(Community& c) {
  return (local_clust_coef(c) * c.size)/c.g[boost::graph_bundle].n;
}

bool val = false;
QualityFunction q_val = Modularity;
void init_experiments (bool v /*=false*/, QualityFunction q /*=Modularity*/) {
  val = v;
  q_val = q;
}

double global_mod=0;
double global_cond=0;
double global_comp=0;
double global_size=0;
double global_cc=0;

double median_degree=-1;
void init_experiments_create(Community& c) {
  c.vol = c.volume();
  c.diameter = 0;
  if(Parameters::isExpToDo(Modularity, Global)) {
    global_mod += modularity(c);
  }
  if(Parameters::isExpToDo(Size, Global)) {
    global_size += 1;
  }
  c.avg_clust = 0;
}

long iteration = 1;

double merged_community_experiments(Community& c, std::vector<Community*>& map_vertex_community) {
  iteration++;
  bool out_single = false;
  bool out_global = false;

  if(out_single)
    Parameters::getExpOutStream() << c.size;

  if(Parameters::isExpToDo(Conductance, Local) || Parameters::isExpToDo(Conductance, Global)) {
    double cond = conductance(c);
    if(out_single)
      Parameters::getExpOutStream() << " " << local_conductance(c);
    if(Parameters::isExpToDo(Conductance, Global)) {
      global_cond += cond;
    }
  }

  if(Parameters::isExpToDo(Modularity, Local) || Parameters::isExpToDo(Modularity, Global)) {
    double mod = modularity(c);
    if(Parameters::isExpToDo(Modularity, Global)) {
      global_mod += mod;
    }
  }

  if(Parameters::isExpToDo(Compactness, Local) || Parameters::isExpToDo(Compactness, Global)) {
    c.diameter = diameter(c, map_vertex_community);
    double comp = compactness(c);
    if(out_single)
      Parameters::getExpOutStream() << " " << c.diameter;
    if(Parameters::isExpToDo(Compactness, Global)) {
      global_comp += comp;
    }
  }

  if(Parameters::isExpToDo(ClustCoef, Local) || Parameters::isExpToDo(ClustCoef, Global)) {
    double cc = clust_coef(c);
    if(out_single)
      Parameters::getExpOutStream() << " " << local_clust_coef(c);
    if(Parameters::isExpToDo(ClustCoef, Global)) {
      global_cc += cc;
    }
  }

  if(Parameters::isExpToDo(Size, Local) || Parameters::isExpToDo(Size, Global)) {
    if(Parameters::isExpToDo(Size, Global)) {
      global_size += 1;
    }
  }
  if(out_single)
    Parameters::getExpOutStream() << std::endl;


  //Is there a need to output the global values
  if(out_global) {
    Parameters::getExpOutStream() << iteration;
    if(Parameters::isExpToDo(Conductance, Global)) {
      Parameters::getExpOutStream() << " " << global_cond;
    }
    if(Parameters::isExpToDo(Modularity, Global)) {
      Parameters::getExpOutStream() << " " << global_mod;
    }
    if(Parameters::isExpToDo(Compactness, Global)) {
      Parameters::getExpOutStream() << " " << global_comp;
    }
    if(Parameters::isExpToDo(ClustCoef, Global)) {
      Parameters::getExpOutStream() << " " << global_cc;
    }
    if(Parameters::isExpToDo(Size, Global)) {
      Parameters::getExpOutStream() << " " << global_size;
    }
    Parameters::getExpOutStream() << std::endl;
  }
  if(val) {
    if (q_val == Modularity)
      return global_mod;
    else if (q_val == Conductance)
      return global_cond;
    else if (q_val == Compactness)
      return global_comp;
    else if (q_val == ClustCoef)
      return global_cc;
    else if (q_val == Size)
      return global_size;
  }
  return 0.0;
}

void delete_community_experiments(Community& c) {
  if(Parameters::isExpToDo(Size, Global))
    global_size -= 1;
  if(Parameters::isExpToDo(Modularity, Global))
    global_mod -= modularity(c);
  if(Parameters::isExpToDo(Conductance, Global))
    global_cond -= conductance(c);
  if(Parameters::isExpToDo(Compactness, Global))
    global_comp -= compactness(c);
  if(Parameters::isExpToDo(ClustCoef, Global))
    global_cc -= clust_coef(c);
}
 
void mark_inter_edges(Community& c1, Community& c2, std::vector<Community*>& map_vertex_community, long mark) {
  for(std::list<vertex_t>::iterator iter = c2.vertices.begin(); iter!=c2.vertices.end(); iter++) {
    std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(*iter, c1.g);
    for( ; it.first != it.second; ++it.first) {
      vertex_t neigh = *(it.first);
      if(map_vertex_community[neigh] == &c1) { 
	edge_t e = boost::edge(*iter, neigh, c1.g).first;
	c1.g[e].iteration = mark;
      }
    }
  }
}
  
//Computes the new clustering coefficient of a node if the community c merges with his
double new_local_clust(vertex_t v, Community& c, std::vector<Community*>& map_vertex_community) {
  Community* c_own = map_vertex_community[v];
  std::vector<vertex_t> neis;
  std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(v, c_own->g);
  for( ; it.first != it.second; ++it.first) {
    vertex_t neigh = *(it.first);
    if(map_vertex_community[neigh] == c_own || map_vertex_community[neigh] == &c) { 
      neis.push_back(neigh);
    }
  }
  long tr = 0;
  for (long i = 0; i < neis.size(); i++) {
    for(long j = i+1; j < neis.size(); j++) {
      if(boost::edge(neis[i], neis[j], c_own->g).second) {
	tr++;
      }
    }
  }
  if(neis.size() > 1)
    return ((double)tr*2)/(neis.size()*(neis.size() - 1));
  else 
    return 0.0;
}

//Computes the new average clustering coefficient
double new_clust(Community& c1, Community& c2, std::vector<Community*>& map_vertex_community) {
  std::set<vertex_t> modified_vertex;
  for(std::list<vertex_t>::iterator iter = c2.vertices.begin(); iter!=c2.vertices.end(); iter++) {
    vertex_t v = *iter;
    std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(v, c1.g);
    bool modified = false;
    for( ; it.first != it.second; ++it.first) {
      vertex_t neigh = *(it.first);
      if(map_vertex_community[neigh] == &c1) {
	if(modified_vertex.find(neigh) == modified_vertex.end()) {
	  double clust_prev = new_local_clust(neigh, c1, map_vertex_community);
	  double clust_new = new_local_clust(neigh, c2, map_vertex_community);
	  c1.avg_clust += (clust_new - clust_prev)/c1.size;
	  modified_vertex.insert(neigh);
	}
	modified = true;
      }
    }
    if(modified) {
      double clust_prev = new_local_clust(v, c2, map_vertex_community);
      double clust_new = new_local_clust(v, c1, map_vertex_community);
      c2.avg_clust += (clust_new - clust_prev)/c2.size;
    }
  }
  return (c1.avg_clust*c1.size + c2.avg_clust*c2.size)/(c1.size + c2.size);
}

void merge_community_experiments(Community& c1, Community& c2, std::vector<Community*>& map_vertex_community) {
  c1.vol += c2.vol;
  c1.nb_edges += c2.nb_edges + c1.nb_inter_edges(c2, map_vertex_community);
  mark_inter_edges(c1, c2, map_vertex_community, iteration);
  if(c1.vertices.size() == 1)
    c1.g[c1.vertices.front()].iteration = iteration;
  if(c2.vertices.size() == 1)
    c2.g[c2.vertices.front()].iteration = iteration;
  if(Parameters::isExpToDo(ClustCoef, Global) || Parameters::isExpToDo(ClustCoef, Local))
    c1.avg_clust = new_clust(c1, c2, map_vertex_community);
}

/*c1 is the community which gathers both communities*/
void pre_merged_community_experiments(Community& c1, Community& c2, std::vector<Community*>& map_vertex_community) {
  delete_community_experiments(c1);
  delete_community_experiments(c2);
  merge_community_experiments(c1, c2, map_vertex_community);
}
