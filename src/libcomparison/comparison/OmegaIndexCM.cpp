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
#include "OmegaIndexCM.hpp"
#include <map>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>
#include "sets.hpp"

OmegaIndexCM::OmegaIndexCM() {
  name = "omega_index";
	std::stringstream s;
  s << std::dec << NB_SAMPLES;
  description = "The Omega index measure. See Xie et al., 'Overlapping community detection in networks: The state-of-the-art and comparative study', 2013. Warning : the method uses " + s.str() + " random samples if the binomial of the number of elements is bigger.";
}

OmegaIndexCM::~OmegaIndexCM() {
}


long common_clusts(long v1, long v2, std::list<long>& memb1, std::list<long>& memb2) {
	long sum = 0;
	for(std::list<long>::iterator it1 = memb1.begin(); it1 != memb1.end(); it1++) {
		for(std::list<long>::iterator it2 = memb2.begin(); it2 != memb2.end(); it2++) {
			if(*it1 == *it2)
				sum++;
		}
	}
	return sum;
}

double OmegaIndexCM::apply(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2) {
	std::vector<long> res1, res2;
	pair_distribution(memb1, res1);
	pair_distribution(memb2, res2);

	long min = res1.size();
	if(res1.size() > res2.size())
		min = res2.size();
	long sum = 0;
	for(long i = 0; i < min; i++) {
		sum = res1[i]*res2[i];
	}
	double n = memb1.size();
	double o_e = 4*sum/(n*n*(n-1)*(n-1));
	double o_u = 0.0;
	if(NB_SAMPLES < memb1.size() * memb1.size())
		o_u = approximate_omega_u(memb1, memb2, NB_SAMPLES);
	else
		o_u = omega_u(memb1, memb2);
	double res = (o_u - o_e)/(1 - o_e);
	return res;
}

double OmegaIndexCM::omega_u(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2) {
	long sum = 0;
	for(long i = 0; i < memb1.size(); i++) {
		for(long j = i+1; j < memb1.size(); j++) {
			if(common_clusts(i, j, memb1[i], memb1[j]) == common_clusts(i, j, memb2[i], memb2[j]))
				sum++;
		}
	}
	return 2*sum/((double)(memb1.size() * (memb1.size() - 1)));
}

double OmegaIndexCM::approximate_omega_u(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2, long nb_samples) {
	long sum = 0;
	srand (time(NULL));
	for(long i = 0; i < nb_samples; i++) {
		long v1 = rand() % memb1.size();
		long v2 = rand() % memb2.size();
		if(common_clusts(v1, v2, memb1[v1], memb1[v2]) == common_clusts(v1, v2, memb2[v1], memb2[v2])) {
			sum++;
		}
	}
	return sum/((double)nb_samples);
}

OmegaIndexCM::Node::Node(std::set<long>& inter) {	
	cardinal = 0;
	clusts = inter;
	SearchTree c;
	children = c;
	str_clusts = "";
}

void OmegaIndexCM::Node::addChild(Node* n) {
	children.insert(n);
}

std::string OmegaIndexCM::Node::to_str() {
	std::string res;
	if (str_clusts == "") {
		std::stringstream strstream;
		for(std::set<long>::iterator it = clusts.begin(); it != clusts.end(); it++)
			strstream << *it << " ";
		res = strstream.str();
		str_clusts = res;
	} else
		res = str_clusts;
	return res;
}

OmegaIndexCM::SearchTree::SearchTree() {
	std::map<long, SearchTree> b;
	branches = b;
	node = NULL;
	root = true;
}

OmegaIndexCM::SearchTree::~SearchTree() {
	// if(node != NULL)
	// 	delete node;
}

void OmegaIndexCM::SearchTree::insert(Node* n, std::set<long>& clusts) {
	if(!root && clusts.size() == 0) {
		node = n;
	} else if(clusts.size() > 0) {
		long c = *(clusts.begin());
		clusts.erase(clusts.begin());
		if(branches.find(c) == branches.end()) {
			SearchTree st;
			st.root = false;
			branches[c] = st;
		}
		branches[c].insert(n, clusts);
	}
}

void OmegaIndexCM::SearchTree::insert(Node* n) {
	std::set<long> cl = n->clusts;
	insert(n, cl);
}

void OmegaIndexCM::SearchTree::get_all(std::deque<Node*>& results) {
	if(branches.size() == 0 && node != NULL) {
		results.push_back(node);
	} else {
		for(std::map<long, SearchTree>::iterator branches_iter = branches.begin(); branches_iter != branches.end(); branches_iter++) {
			(branches_iter->second).get_all(results);
		}
	}
}

void OmegaIndexCM::SearchTree::find(std::set<long>& inter, std::deque<Node*>& results) {
	if(branches.size() == 0 && node != NULL) {
		results.push_back(node);
	} else {
		//We either loop on the intersections or on the branches, depending on the container size
		//in order to take full profit of the logarithmic search
		if(inter.size() < branches.size()) {
			for(std::set<long>::iterator inter_iter = inter.begin(); inter_iter != inter.end(); inter_iter++) {
				if(branches.find(*inter_iter) != branches.end())
					branches[*inter_iter].find(inter, results);
			}
		} else {
			for(std::map<long, SearchTree>::iterator branches_iter = branches.begin(); branches_iter != branches.end(); branches_iter++) {
				if(inter.find(branches_iter->first) != inter.end())
					(branches_iter->second).find(inter, results);
			}
		}
	}
}

bool comp_vertices(std::pair<long, long> a, std::pair<long, long> b) {
	return a.second < b.second;
}

void OmegaIndexCM::pair_distribution(std::vector<std::list<long> >& memb, std::vector<long>& results) {
	//Sort nodes by number of membership
	//Vector is much faster than list for pushing, sorting small elements (<~150bytes) and iterating through it
	//Therefore, even though a list would make more sense algorithmically, we chose a vector
	std::vector<std::pair<long, long> > sorted_vertices;
	
	for(long i = 0; i < memb.size(); i++) {
		long size = memb[i].size();
		if(size > 0) {
			sorted_vertices.push_back(std::make_pair(i, size));
		}
	}
	std::sort(sorted_vertices.begin(), sorted_vertices.end(), comp_vertices);

	//Create the search tree of the root nodes (initially empty)
	SearchTree root;

	//Loop over all nodes in the order of their size of membership
	std::deque<Node*> nodes;
	long depth = 1;
	for(long i = 0; i < sorted_vertices.size(); i++) {
		long v = sorted_vertices[i].first;
	  //Create a set from the list (easier to find elements)
		std::set<long> cl;
		for(std::list<long>::iterator it = memb[v].begin(); it != memb[v].end(); it++) {
			cl.insert(*it);
		}

		//Find all root nodes that are ancestors of the vertex
		std::deque<Node*> ances;
		root.find(cl, ances);
		
		//If there is no ancestor, the vertex creates a root node
		if(ances.size() == 0) {
			Node* n = new Node(cl);
			nodes.push_back(n);
			n->cardinal = 1;
			n->cardinal_without_inter = 1;
			n->cardinal_union = 1;
			root.insert(n);
			if(n->clusts.size()+1 > depth)
				depth = n->clusts.size()+1;
		} else {
			//Find the node in the tree that matches the clusters
			std::set<std::string> visited;
			std::deque<Node*> to_visit;
			for(std::deque<Node*>::iterator it_a = ances.begin(); it_a != ances.end(); it_a++) {
				to_visit.push_back(*it_a);
			}
			//std::cout << "4" << std::endl;
			std::deque<Node*> children, leafs;
			while(to_visit.size() > 0) {
				Node* n = to_visit.back();
				to_visit.pop_back();
				n->cardinal++;
				n->cardinal_union++;
				//std::cout << "v : " << i << ", node : " << n->to_str() << " car union : " << n->cardinal_union << std::endl;
				n->children.find(cl, children);
				bool visit_child = false;
				for(std::deque<Node*>::iterator it = children.begin(); it != children.end(); it++) {
					//If it has not been visited yet, we add it to visit
					if(visited.find((*it)->to_str()) == visited.end()) {
						visited.insert((*it)->to_str());
						to_visit.push_back(*it);
						visit_child = true;
					}
				}
				//If there was no visited child, the node is a leaf
				if(!visit_child)
					leafs.push_back(n);
			}
			
			//If there are multiple leafs, we need to create a node
			if(leafs.size() > 1) {
				Node* n = new Node(cl);
				nodes.push_back(n);
				n->cardinal = 1;
				n->cardinal_without_inter = 1;
				long nbpar = 0, sumpar = 0;
				for(std::deque<Node*>::iterator it = leafs.begin(); it != leafs.end(); it++) {
					(*it)->addChild(n);
					nbpar++;
					sumpar += (*it)->cardinal_union;
				}
				n->cardinal_union = sumpar + 1 - nbpar;
				//We update the depth
				if(n->clusts.size()+1 > depth)
					depth = n->clusts.size()+1;
			} else {
				leafs.back()->cardinal_without_inter++;
			}
		}
	}

	results.resize(depth);
	for(long i = 0; i < depth; i++)
		results[i] = 0;
	
	std::deque<Node*> to_visit;
	root.get_all(to_visit);

	//We need to account for the pairs with 0 clusters in common
	for(std::deque<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		//std::cout << "clust : " << (*it)->to_str() << " res0 : " << (*it)->cardinal_without_inter * (sorted_vertices.size() - (*it)->cardinal_union) / 2 << std::endl;
		results[0] +=  (*it)->cardinal_without_inter * (sorted_vertices.size() - (*it)->cardinal_union);
	}

	std::set<std::string> visited;
	while(to_visit.size() > 0) {
		Node* n = to_visit.back();
		to_visit.pop_back();
		long c = n->cardinal_without_inter, c_i = n->cardinal - n->cardinal_without_inter;
		//std::cout << n->to_str() << " : " << n->clusts.size() << " " << c << " " << c_i << " " << n->cardinal_union << " participation : " << c*(c-1) + c*c_i << std::endl;
		//std::cout << "before size : " << n->clusts.size() << " " << depth << std::endl;
		results[n->clusts.size()] += c*(c-1) + 2*c*c_i;
		//std::cout << "after" << std::endl;
		std::deque<Node*> temp;
		n->children.get_all(temp);
		for(std::deque<Node*>::iterator it = temp.begin(); it != temp.end(); it++) {
			if(visited.find((*it)->to_str()) == visited.end()) {
				to_visit.push_back(*it);
				visited.insert((*it)->to_str());
			}
		}
	}

	// for(long i = 0; i < depth; i++)
	// 	std::cout << results[i] << " ";
	// std::cout << std::endl;

	for(std::deque<Node*>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
		delete *iter;
	}
}
