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
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <deque>
#include <cstring>
#include <sstream>
#include <fstream>
#include <cstdlib>

void usage(std::string more) {
  std::cerr << more << std::endl;
  std::cerr << "usage: k-core infile outfile [k]" << std::endl << std::endl;
  std::cerr << "Gives the k-core clustering of the graph" << std::endl;
  std::cerr << "infile\tThe input graph (incidence list format, for each edge (v1, v2),  v1 v2\\n)" << std::endl;
  std::cerr << "outfile\tThe output clustering : each line is a vertex and a cluster id" << std::endl;
  std::cerr << "k\tThe parameter of the k-core decomposition (default : 3)" << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  //std::cout << "1" << std::endl;
  unsigned int k = 3;
  std::string s_infile = "";
  std::string s_outfile = "";
  if(argc > 2) {
    s_infile = argv[1];
    s_outfile = argv[2];
    if(argc > 3)
      k = atoi(argv[3]);
  } else
    usage("Not enough arguments");

  std::ifstream infile;
  infile.open(s_infile.c_str(), std::ifstream::in);

  //std::cout << "2" << std::endl;

  //Reading file, populating adjacency map
  std::map<long, std::list<long> > adjacency;
  std::string line;
  unsigned long max_degree = 0;
  while (std::getline(infile, line)) {
    if(line != "") {
      long from, to;
      sscanf(line.c_str(), "%li %li", &from, &to);

      if(adjacency.find(from) == adjacency.end())
	adjacency[from] = std::list<long>();
      adjacency[from].push_back(to);
      if(adjacency[from].size() > max_degree)
	max_degree = adjacency[from].size();

      if(adjacency.find(to) == adjacency.end())
	adjacency[to] = std::list<long>();
      adjacency[to].push_back(from);
      if(adjacency[to].size() > max_degree)
	max_degree = adjacency[to].size();
    }
  }

  //std::cout << "3" << std::endl;

  //Creating degree table
  std::vector<std::list<long> > degree(max_degree+1);
  for(unsigned long i = 0; i < degree.size(); i++)
    degree[i] = std::list<long>();

  std::map<long, std::list<long>::iterator> vertex_to_pos;
  long v;
  unsigned long d;
  unsigned long min_degree = max_degree;
  for(std::map<long, std::list<long> >::iterator iter = adjacency.begin(); iter != adjacency.end(); iter++) {
    v = iter->first;
    d = iter->second.size();

    //std::cout << "degree " << v << " : " << d << std::endl;

    if(d < min_degree)
      min_degree = d;

    degree[d].push_back(v);
    vertex_to_pos[v] = --(degree[d].end());
  }

  //std::cout << "4" << std::endl;

  //Removing successively low-degree nodes
  std::list<long> l;
  while(min_degree < k) {
    //std::cout << "4.1" << std::endl;
    long v = degree[min_degree].back();
    degree[min_degree].pop_back();
    //std::cout << "4.2 v : " << v << std::endl;
    l = adjacency[v];
    for(std::list<long>::iterator iter = adjacency[v].begin(); iter != adjacency[v].end(); iter++) {
      //std::cout << "4.2.1 " << *iter << std::endl;
      long u = *iter;
      unsigned long d_prev = adjacency[u].size();
      unsigned long d_new = d_prev - 1;
      adjacency[u].remove(v);
      //std::cout << "4.2.2" << std::endl;
      degree[d_prev].erase(vertex_to_pos[u]);
      degree[d_new].push_back(u);
      //std::cout << "4.2.3" << std::endl;
      vertex_to_pos[u] = --(degree[d_new].end());
      if(d_new < min_degree)
	min_degree = d_new;
    }
    //std::cout << "4.3" << std::endl;
    adjacency[v].clear();

    while(degree[min_degree].size() == 0)
      min_degree++;
  }

  //std::cout << "5" << std::endl;


  //Find connected components, write in the output file
  unsigned long c = 0; //Community identifier

  std::map<long, long> visited;
  for(std::map<long, std::list<long> >::iterator iter = adjacency.begin(); iter != adjacency.end(); iter++)
    visited[iter->first] = -1;
 
  long u;
  for(std::map<long, std::list<long> >::iterator iter = adjacency.begin(); iter != adjacency.end(); iter++) {
    v = iter->first;
    if(visited[v] < 0) {
      std::deque<long> q;
      q.push_back(v);
      while(!q.empty()) {
	u = q.back();
	q.pop_back();
	visited[u] = c;
	for(std::list<long>::iterator iter = adjacency[u].begin(); iter != adjacency[u].end(); iter++) {
	  if(visited[*iter] < 0)
	    q.push_back(*iter);
	}
      }
      c++;
    }
  }

  std::ofstream outfile;
  outfile.open(s_outfile.c_str());

  for(std::map<long, long>::iterator iter = visited.begin(); iter != visited.end(); iter++) { 
    outfile << iter->first << " " << iter->second << std::endl;
  }
  return 0;
}
