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
#include "CompactnessSCQF.hpp"
#include <deque>
#include <map>
#include <iostream>
#include <cstdlib>

CompactnessSCQF::CompactnessSCQF() {
  name = "compactness";
  description = "The compactness, from Creusefond et al., 'Finding compact communities in large graphs', 2015";
}

int dist(igraph_t* graph, long root, std::vector<long> & membership, std::deque<long> & nodes, long community) {
  std::deque<long> queue;
  queue.push_back(root);

  std::map<long, int> distances;
  for(std::deque<long>::iterator iter = nodes.begin();  iter != nodes.end(); iter++)
    distances[*iter] = -1;
  distances[root] = 0;
  int max_dist = 0;

  igraph_vector_t neis;
  igraph_vector_init(&neis, 0);
  while(!queue.empty()) {
    long v = queue.back();
    queue.pop_back();
    int distance = distances[v];

    igraph_neighbors(graph, &neis, v, IGRAPH_ALL);
    for(long i = 0; i < igraph_vector_size(&neis); i++) {
      igraph_integer_t u = igraph_vector_e(&neis, i);
      if(membership[u] == community && distances[u] == -1) {
	distances[u] = distance + 1;
	queue.push_front(u);
	if(distance + 1 > max_dist)
	  max_dist = distance + 1;
      }
    }
  }
	igraph_vector_destroy(&neis);
  return max_dist;
}

long nb_edges(igraph_t* graph, std::vector<long> & membership, std::deque<long> & nodes, long community) {
  long total = 0;
  igraph_vector_t neis;
  igraph_vector_init(&neis, 0);
  for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
    long v = *iter;
    igraph_neighbors(graph, &neis, v, IGRAPH_ALL);

    for(long i = 0; i < igraph_vector_size(&neis); i++) {
      igraph_integer_t u = igraph_vector_e(&neis, i);
      if(membership[u] == community && u > v) {
	total += 1;
      }
    }
  }
  return total;
}

double CompactnessSCQF::apply(igraph_t* graph, std::vector<long> & membership, std::deque<long> & nodes) {
  int diam = 0;
  long community = membership[nodes.front()];
  // for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
  //   if(membership[*iter] != community) {
  //     std::cout << membership[*iter] << " " << community << std::endl;
  //     exit(1);
  //   }
  // }

  long nb_samples = 10;

  if(nodes.size() > 10) {
    for(long i = 0; i < nb_samples; i++) {
      long select = rand() % nodes.size();
      long j = 0;
      for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
	if(j == select) {
	  int tmp = dist(graph, *iter, membership, nodes, community);
	  if(tmp > diam)
	    diam = tmp;
	}
	j++;
      }
    }
  } else {
    for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
      int tmp = dist(graph, *iter, membership, nodes, community);
      if(tmp > diam)
	diam = tmp;
    }
  }

  // for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
  //   int tmp = dist(graph, *iter, membership, nodes, community);
  //   if(tmp > diam)
  //     diam = tmp;
  // }
  if(diam > 0)
    return ((double)nb_edges(graph, membership, nodes, community))/diam;
  else
    return 0;
}
