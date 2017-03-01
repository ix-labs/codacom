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
#include "CommonIntervalDetection.hpp"
#include "CommunityDetection.hpp"

typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;

class TreeElement {
public:
  TreeElement(long min, long max): children(), interval_min(min), interval_max(max), prime(true), increasing(true){}

  long computeMin() {
    std::list<TreeElement>::iterator iter;
    if(children.size() > 0) {
      iter = children.begin();
      long result = (*iter).computeMin();
      iter++;
      for( ; iter!=children.end(); iter++) {
	if(result > (*iter).computeMin())
	  result = (*iter).computeMin();
      }
      return result;
    }
    return interval_max;
  }

  long getMin() {
    return interval_min;
  }
  long getMax() {
    return interval_max;
  }
  long nbChildren() {
    return children.size();
  }
  void addChild(TreeElement& t) {
    children.push_back(t);
    if(t.getMin() < interval_min) {
      interval_min = t.getMin();
    }
    if(t.getMax() > interval_max) {
      interval_max = t.getMax();
    }
  }
  void setMonotonic(bool _increasing) {
    prime = false;
    increasing = _increasing;
  }
  bool isPrime() {
    return prime;
  }
  bool isIncreasing() {
    return increasing;
  }
  std::string toString() {
    std::string s = "";
    std::list<TreeElement>::iterator iter;
    if(children.size() > 0) {
      s += "(";
      iter = children.begin();
      s += iter->toString();
      iter++;
      for( ; iter!=children.end(); iter++) {
	s += ", " + iter->toString();
      }
      s += ")";
    } else
      s += std::to_string(interval_max);
    return s;
  }
  std::list<TreeElement>& getChildren() {
    return children;
  }
private:
  std::list<TreeElement> children;
  long interval_min, interval_max;
  bool prime;
  bool increasing;
};
  
bool tryExtension(std::vector<TreeElement>& forest) {
  TreeElement* e1 = &(forest[forest.size()-2]);
  TreeElement* e2 = &(forest[forest.size()-1]);
  
  if(e1->getMin() == e2->getMax()+1) {
    if(!e1->isPrime() && !e1->isIncreasing()) {
      e1->addChild(*e2);
      forest.pop_back();
      return true;
    }
  }
  if(e1->getMax() == e2->getMin()-1) {
    if(!e1->isPrime() && e1->isIncreasing()) {
      e1->addChild(*e2);
      forest.pop_back();
      return true;
    }
  }
  return false;
}

bool tryMonotonicCreation(std::vector<TreeElement>& forest) {
  bool created = false;
  if(forest.size() > 1) {
    TreeElement* e1 = &(forest[forest.size()-2]);
    TreeElement* e2 = &(forest[forest.size()-1]);
    TreeElement* e_new;
    if(e1->getMin() == e2->getMax()+1) {
      e_new = new TreeElement(e2->getMin(), e1->getMax());
      e_new->setMonotonic(false);
      created = true;
    }
    if(e1->getMax() == e2->getMin()-1) {
      e_new = new TreeElement(e1->getMin(), e2->getMax());
      e_new->setMonotonic(true);
      created = true;
    }
    if(created) {
      e_new->addChild(*e1);
      e_new->addChild(*e2);
      forest.pop_back();
      forest.pop_back();
      forest.push_back(*e_new);
    }
  }
  return created;
}

void tryPrimeCreation(std::vector<TreeElement>& forest, long index) {
  TreeElement* e_new;
  long max = forest.back().getMax();
  long min = forest.back().getMin();
  e_new = new TreeElement(min, max);
  while(forest.size() > (unsigned)index) {
    e_new->addChild(forest.back());
    forest.pop_back();
  }
  forest.push_back(*e_new);
}

//sort in increasing order
bool compare_visited_vertex(const VertexProperties* first, const VertexProperties* second) {
  return first->visited < second->visited;
}

//sort in increasing order
bool compare_visited_prec_vertex(const VertexProperties* first, const VertexProperties* second) {
  return first->visited_preced < second->visited_preced;
}

void initMinGreaterMaxSmaller(std::vector<long>& minGreaterOnLeft, std::vector<long>& maxSmallerOnLeft, std::vector<long>& permutation) {
  long n = permutation.size();
  std::list<long> index_not_found_GOL; //A linked list of the indices that haven't been found in the computation of minGreaterOnLeft
  std::list<long> index_not_found_SOL; //Same for maxSmallerOnLeft
  std::vector<std::list<long>::iterator> iterators_GOL(n);
  std::vector<std::list<long>::iterator> iterators_SOL(n);

  for(long i=0; i<n; i++) {
    index_not_found_GOL.push_back(i);
    iterators_GOL[i] = --index_not_found_GOL.end();
    index_not_found_SOL.push_back(i);
    iterators_SOL[i] = --index_not_found_SOL.end();
    minGreaterOnLeft[i] = n;
    maxSmallerOnLeft[i] = -1;
  }
  for(long i = n - 1; i>=0; i--) {
    long p_i = permutation[i];
    std::list<long>::iterator spot_GOL = iterators_GOL[p_i];
    std::list<long>::iterator previous_spot = spot_GOL;
    previous_spot--;
    if(spot_GOL != index_not_found_GOL.begin())
      maxSmallerOnLeft[i] = *(previous_spot);
    
    index_not_found_GOL.erase(spot_GOL);
    std::list<long>::iterator spot_SOL = iterators_SOL[p_i];
    std::list<long>::iterator next_spot = spot_SOL;
    next_spot++;
    if(next_spot != index_not_found_SOL.end())
      minGreaterOnLeft[i] = *next_spot;
    index_not_found_SOL.erase(spot_SOL);
  }
}

void init_permutation(Graph& g, std::vector<long>& permutation, std::vector<VertexProperties*>& index_to_vertex) {
  std::vector<VertexProperties*> nodes_visited_prec(g[boost::graph_bundle].n);
  std::pair<vertex_iter, vertex_iter> vp;
  long i = 0;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    index_to_vertex[i] = &(g[*vp.first]);
    nodes_visited_prec[i] = &(g[*vp.first]);
    i++;
  }
  std::sort(index_to_vertex.begin(), index_to_vertex.end(),compare_visited_vertex);
  std::sort(nodes_visited_prec.begin(), nodes_visited_prec.end(),compare_visited_prec_vertex);

  std::map<VertexProperties*,long> vertex_to_indice;
  for(long i=0; i<g[boost::graph_bundle].n; i++) 
    vertex_to_indice[index_to_vertex[i]] = i;

  for(long i=0; i<g[boost::graph_bundle].n; i++)
    permutation[i] = vertex_to_indice[nodes_visited_prec[i]];
}

void recursive_merge(TreeElement& t, std::vector<Community*>& map_vertex_community, std::vector<VertexProperties*>& index_to_vertex) {
  std::list<TreeElement> c = t.getChildren();
  if(c.size() > 0) {
    std::list<TreeElement>::iterator iter=c.begin();
    for(; iter!=c.end(); iter++)
      recursive_merge(*iter, map_vertex_community, index_to_vertex);
    
    //std::cout << "Begin treatment "<< t.toString() << " size : "<< c.size() << std::endl;
    //std::cout << "1"<< std::endl;
    Community* c1 = map_vertex_community[index_to_vertex[c.front().getMin()]->descriptor];
    delete_community_experiments(*c1);
    iter=c.begin();
    iter++;
    //std::cout << "2"<< std::endl;
    for(; iter!=c.end(); iter++) {
      //std::cout << "2.1"<< std::endl;
      Community* c2 = map_vertex_community[index_to_vertex[(*iter).getMin()]->descriptor];
      //std::cout << "Mins : " << c.front().getMin() << " " << (*iter).getMin() << std::endl;
      //std::cout << "Descriptors : " << index_to_vertex[c.front().getMin()]->descriptor << " " << index_to_vertex[(*iter).getMin()]->descriptor << std::endl;
      //std::cout << "Merge " << c1 << " with " << c2 << " size : "  << c1->vertices.size()<< " " << c2->vertices.size() << std::endl; 
      //std::cout << "Merge : " << index_to_vertex[c.front().getMin()]->label << " " << index_to_vertex[(*iter).getMin()]->label << std::endl;
      delete_community_experiments(*c2);
      merge_community_experiments(*c1, *c2, map_vertex_community);
      c1->merge(*c2, map_vertex_community);
      //std::cout << "2.2"<< std::endl;
    }
    merged_community_experiments(*c1, map_vertex_community);
    //std::cout << "End treatment "<< t.toString() << " size : "<< c.size() << std::endl;
  }
}

void tree_to_communities(Graph& g, TreeElement& t, std::vector<VertexProperties*>& index_to_vertex) {
  long n = index_to_vertex.size();
  std::vector<Community*> map_vertex_community(n);
  
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    Community* c = new Community(*vp.first, g);
    init_experiments_create(*c);
    g[*vp.first].descriptor = *vp.first;
    map_vertex_community[*vp.first] = c; //uses the fact that vertex_t is a [0;n[ long
  }

  init_experiments();
  
  recursive_merge(t, map_vertex_community, index_to_vertex);
}

void common_intervals_clustering(Graph& g) {
  //Creation of the permutation
  long n = g[boost::graph_bundle].n;
  //long n = 11;
  std::vector<VertexProperties*> nodes_visited(n);
  
  std::vector<long> permutation(n);
  std::vector<long> indice_to_tree(n); //From I to the indice in the forest vector
  std::vector<TreeElement> forest;
  
  init_permutation(g, permutation, nodes_visited);
  std::cout << "Descriptors : ";
  for(long i = 0; i<n; i++)
    std::cout << nodes_visited[i]->label;
  std::cout << std::endl;
  std::cout << "Permutation : ";
  for(long i = 0; i<n; i++)
    std::cout << permutation[i];
  std::cout << std::endl;
  // permutation[0] = 0;
  // permutation[1] = 1;
  // permutation[2] = 11;
  // permutation[3] = 10;
  // permutation[4] = 5;
  // permutation[5] = 4;
  // permutation[6] = 8;
  // permutation[7] = 9;
  // permutation[8] = 2;
  // permutation[9] = 3;
  // permutation[10] = 6;
  // permutation[11] = 7;

  // permutation[0] = 0;
  // permutation[1] = 4;
  // permutation[2] = 5;
  // permutation[3] = 1;
  // permutation[4] = 2;
  // permutation[5] = 3;
  // permutation[6] = 6;

  // permutation[0] = 0;
  // permutation[1] = 1;
  // permutation[2] = 2;
  // permutation[3] = 8;
  // permutation[4] = 3;
  // permutation[5] = 4;
  // permutation[6] = 7;
  // permutation[7] = 6;
  // permutation[8] = 5;
  // permutation[9] = 9;
  // permutation[10] = 10;

  //Computation of minGreaterOnLeft & maxSmallerOnLeft
  std::vector<long> minGreaterOnLeft(n);
  std::vector<long> maxSmallerOnLeft(n);
  initMinGreaterMaxSmaller(minGreaterOnLeft, maxSmallerOnLeft, permutation);

  std::vector<long> potBeg;
  std::vector<long> minBefore;
  std::vector<long> maxBefore;
  for(long y = 0; y<n; y++) {
    long p_y = permutation[y];
    long mini = p_y;
    long maxi = p_y;
    //std::cout << "3.0\n";
    while(potBeg.size() > 0 && (minGreaterOnLeft[potBeg.back()] < p_y || maxSmallerOnLeft[potBeg.back()] > p_y)) {
      mini = std::min(mini, minBefore.back());
      maxi = std::max(maxi, maxBefore.back());
      potBeg.pop_back();
      minBefore.pop_back();
      maxBefore.pop_back();
    }
    //std::cout << "3.1\n";
    potBeg.push_back(y);
    minBefore.push_back(mini);
    maxBefore.push_back(maxi);
    //std::cout << "3.2\n";
    long iPotBeg = potBeg.size()-1;
    long x = potBeg.back();
    mini = p_y;
    maxi = p_y;
    //std::cout << "3.2.1\n";
    //std::cout << "PotBeg : ";
    //for(std::vector<long>::iterator tmp = potBeg.begin(); tmp != potBeg.end(); tmp++)
      //std::cout << *tmp << " ";
    //std::cout << std::endl;
    //std::cout << "minBefore : ";
    //for(std::vector<long>::iterator tmp = minBefore.begin(); tmp != minBefore.end(); tmp++)
      //std::cout << *tmp << " ";
    //std::cout << std::endl;
    //std::cout << "maxBefore : ";
    //for(std::vector<long>::iterator tmp = maxBefore.begin(); tmp != maxBefore.end(); tmp++)
      //std::cout << *tmp << " ";
    //std::cout << std::endl;

    //while(iPotBeg>=0 && (std::max(maxi, forest[indice_to_tree[x]].getMax()) - std::min(mini, forest[indice_to_tree[x]].getMin()) == y-x)) {
    //while(iPotBeg>=0 && (maxi - mini == y-x)) {
    while(iPotBeg>=0 && (std::max(maxi, permutation[x]) - std::min(mini, permutation[x]) == y-x)) {
      //std::cout << "x y : " << x << " " << y << std::endl;
      //std::cout << "Perm : [" << permutation[x] << ";" << permutation[y] << "]\n";
      //std::cout << "Permutation : ";
      //for(long tmp = x; tmp <= y; tmp++)
	//std::cout << permutation[tmp] << " ";
      //std::cout << std::endl;
      
      long shift = 1;
      if(x != y) {
      	if(tryExtension(forest)) {
	  //std::cout << "Extension, shift : " << shift <<"\n";
	} else {
	  //std::cout << "1" << std::endl;
      	  if(tryMonotonicCreation(forest)) {
	    //std::cout << "Monotonic creation increasing ? " << forest.back().isIncreasing() << "\n";
	  } else {
	    if((unsigned)indice_to_tree[x] < forest.size()-1 && forest.size() - indice_to_tree[x] > 3) {
	      //std::cout << "Prime creation, index " << indice_to_tree[x] << " forest size " << forest.size() << "\n";
	      tryPrimeCreation(forest, indice_to_tree[x]);
	    } else {
	      iPotBeg = -1;
	    }
	  }
	  indice_to_tree[x] = forest.size()-1;
	  // potBeg.pop_back();
	  // if(minBefore.back() < minBefore[minBefore.size()-1])
	  //   minBefore[minBefore.size()-1] = minBefore.back();
	  // minBefore.pop_back();
	  // if(maxBefore.back() > maxBefore[maxBefore.size()-1])
	  //   maxBefore[maxBefore.size()-1] = maxBefore.back();
	  // maxBefore.pop_back();
      	}
      } else {
      	forest.push_back(*(new TreeElement(permutation[y], permutation[y])));
	indice_to_tree[x] = forest.size()-1;
	//std::cout << "New el\n";
      }
      // mini = std::min(mini, forest.back().getMin());
      // maxi = std::max(maxi, forest.back().getMax());
      //std::cout << "Forest : [" << forest.back().getMin() << ";" << forest.back().getMax() << "]\n";
      //std::cout << forest.back().toString() << std::endl;
      if(forest.back().getChildren().size() == 1) {
	std::cout << "ERROR 1-CHILD NODE\n";
	exit(1);
      }

      // if(forest.back().computeMin() != forest.back().getMin()) {
      // 	std::cout << "ERROR computed min : " << forest.back().computeMin() << " != min : " << forest.back().getMin() << " size : " <<  forest.back().getChildren().size() << "\n";
      // 	exit(1);
      // }

      //std::cout << "tmp " << iPotBeg-shift << std::endl;

      // while(iPotBeg-shift >= 0 && indice_to_tree[potBeg[iPotBeg-shift]] == forest.size()-1)
      // 	shift++;

      mini = std::min(mini, minBefore[iPotBeg]);
      maxi = std::max(maxi, maxBefore[iPotBeg]);

      iPotBeg -= shift;
      if(iPotBeg>=0) {
    	x = potBeg[iPotBeg];
      }
      // mini = std::min(mini, forest[indice_to_tree[x]].getMin());
      // maxi = std::max(maxi, forest[indice_to_tree[x]].getMax());
      //std::cout << "min : " << mini << " maxi : " << maxi << " forest size : " << forest.size() << std::endl;
      //std::cout << "NextPerm : [" << permutation[x] << ";" << permutation[y] << "]\n";
      //std::cout << std::endl;
    }
  }
  
  std::cout << "Final tree : " << forest.front().toString() << std::endl;
  std::cout << "Size : " << forest.front().getChildren().size() << std::endl;
  tree_to_communities(g, forest.front(), nodes_visited);
  //std::cout << forest.back().toString() << std::endl;
}
