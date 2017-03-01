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
#include "LexDFS.hpp"
#include <iostream>
#include <list>
#include <random>
#include <algorithm>
#include <unordered_map>

class Class {
public:
  long rank;
  std::list<VertexProperties*> vertices;
};

long nb_classes = 1;

bool compare_classes (const Class* first, const Class* second) {
  return first->rank < second->rank;
}

bool compare_vertex(const VertexProperties* first, const VertexProperties* second) {
  return first->visited_preced > second->visited_preced;
}

void split(std::list<Class*>& partition, 
	   std::list<VertexProperties*>& neighbors,
	   bool sort,
	   std::unordered_map<Class*, std::list<Class*>::iterator>& map_class_iterator,
	   std::unordered_map<VertexProperties*, Class*>& map_vertex_class,
	   std::unordered_map<VertexProperties*, std::list<VertexProperties*>::iterator>& map_vertex_iterator) {
  VertexProperties* v;
  std::list<Class*> classes;
  long base_rank = partition.front()->rank;

  while(!neighbors.empty()) {
    //std::cout << "begin neighbor\n";
    v = neighbors.front();
    Class* c_old = map_vertex_class[v];
    std::vector<VertexProperties*> c_new_elems;
    Class* c_new;
    
    std::list<VertexProperties*>::iterator iter = neighbors.begin();
    while(iter != neighbors.end()) { //Going through the rest of the neighbors
      //std::cout << "neighbor\n";
      if(map_vertex_class[*iter] == c_old) {
  	c_old->vertices.erase(map_vertex_iterator[*iter]);
  	c_new_elems.push_back(*iter);
  	iter = neighbors.erase(iter); //Deletes the element from the list and increments the iterator
      } else
  	iter++;
    }

    if(sort) {
      std::sort(c_new_elems.begin(), c_new_elems.end(), compare_vertex);
    } else {
       auto engine = std::default_random_engine{};
       std::shuffle(c_new_elems.begin(), c_new_elems.end(), engine);
    }
    c_new = new Class();
    std::vector<VertexProperties*>::iterator iter2=c_new_elems.begin();
    for(; iter2!=c_new_elems.end(); iter2++) {
      c_new->vertices.push_back(*iter2);
      map_vertex_iterator[*iter2] = --c_new->vertices.end();
      map_vertex_class[*iter2] = c_new;
    }
    
    c_new->rank = c_old->rank;
    //std::cout << "end neighbor\n";
    classes.push_back(c_new);
    if(c_old->vertices.empty()) {
      partition.erase(map_class_iterator[c_old]);
      nb_classes--;
      //std::cout << "Old class deleted, rank : " << c_old->rank << "\n";
      delete c_old;
    }

    //if(partition.isEmpty())
    //std::cout << "Empty\n";
    nb_classes++;
    //std::cout << "New class created\n";
  }
  classes.sort(compare_classes); //Sort with lowest rank first

  std::list<Class*>::iterator i = classes.begin();
  long count = 1;
  while(i != classes.end()) {
    (*i)->rank = base_rank + count;
    count++;
    partition.push_front(*i);
    map_class_iterator[*i] = partition.begin();
    i++;
  }
}

typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;

//If the random is false, it is a lexDFS+, therefore the start parameter is unused
void lexdfs(Graph& g, vertex_t start, bool random) {
  std::pair<vertex_iter, vertex_iter> vp;
  VertexProperties* v;
  std::list<Class*> partition;
  std::unordered_map<Class*, std::list<Class*>::iterator> map_class_iterator;
  std::unordered_map<VertexProperties*, Class*> map_vertex_class;
  std::unordered_map<VertexProperties*, std::list<VertexProperties*>::iterator> map_vertex_iterator;

  //std::cout << "Label initial : " << g[start].label << std::endl;
  if(!random) {
    vp = vertices(g);
    start = *vp.first;
    for (; vp.first != vp.second; ++vp.first) {
      if(g[*vp.first].visited > g[start].visited)
	start = *vp.first;
    }
  }
  
  //Initialisation of the partition and the first class
  v = &(g[start]);
  Class* CInit = new Class();
  partition.push_back(CInit);
  map_class_iterator[CInit] = partition.begin();

  CInit->vertices.push_back(v);
  map_vertex_class[v] = CInit;
  map_vertex_iterator[v] = --CInit->vertices.end();
  
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    VertexProperties* v = &(g[*vp.first]);
    v->descriptor = *vp.first;
    v->visited_preced = v->visited;
    v->visited = -1;
    if(CInit->vertices.front() != v) {
      CInit->vertices.push_back(v);
      map_vertex_class[v] = CInit;
      map_vertex_iterator[v] = --CInit->vertices.end();
    } else {
      //std::cout << "Starting vertex detected, moving on..." << std::endl;
    }
  }
  
  long iteration = 1;
  while(!partition.empty()) {
    Class* C1 = partition.front();

    v = C1->vertices.front();
    C1->vertices.erase(C1->vertices.begin());
    v->visited = iteration;
    //std::cout << v->visited << std::endl;
    //std::cout << "Iter : " << iteration << " Nb classes " << nb_classes << " label " << v->label << std::endl;
    
    if(C1->vertices.empty()) {
      partition.erase(partition.begin());
      delete C1;
      nb_classes--;
      //std::cout << "Taking a new element deleted a class\n";
    }

    iteration++;

    std::pair<adjacency_iterator, adjacency_iterator> it = boost::adjacent_vertices(v->descriptor, g);
    std::list<VertexProperties*> neighbors;
    for( ; it.first != it.second; ++it.first) {
      v = &(g[*it.first]);
      if(v->visited < 0) {
	neighbors.push_back(v);
	//std::cout << "Neighbor found!\n";
      }
    }    
    //std::cout << "neigh size : " << neighbors.size() << std::endl;

    //Split
    if(neighbors.size() > 0) {
      split(partition, neighbors, !random, map_class_iterator, map_vertex_class, map_vertex_iterator);
    }
  }
}
