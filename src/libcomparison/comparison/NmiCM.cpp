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
#include "NmiCM.hpp"
#include <map>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "sets.hpp"

NmiCM::NmiCM() {
  name = "nmi";
  description = "The NMI measure. See Lanichinetti et al., 'Detecting the overlapping and hierarchical community structure in complex networks', 2009.";
}

NmiCM::~NmiCM() {
}


//Computes the number of intersections between two sets
long intersection(std::map<long, bool>& l1, std::map<long, bool>& l2) {
  long inter = 0;
  std::map<long, bool>* p1, *p2;
  if(l1.size() < l2.size()) {
    p1 = &l1;
    p2 = &l2;
  } else {
    p1 = &l2;
    p2 = &l1;
  }
  for(std::map<long, bool>::iterator iter = p1->begin(); iter != p1->end(); iter++)
    if(p2->find(iter->first) != p2->end())
			inter++;
  
  return inter;
}

//to_string et from_string helpers for the map storage
std::string to_string(long i, long j) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(0);
  ss << i << " " << j;
  return ss.str();
}

std::pair<long, long> from_string(std::string s) {
  long i = atol(s.substr(0, s.find(" ")).c_str());
  long j = atol(s.substr(s.find(" ") + 1, s.size()).c_str());
  return std::make_pair(i, j);
}


//Entropy notation
double h(double p) {
  if(p > 0)
    return (-1)*p*log(p);
  else
    return 0;
}

//Entropy of a cluster
double h(std::map<long, bool>& cluster, long n) {
  double p = ((double)cluster.size())/n;
  return h(p) + h(1 - p);
}

//Joint entropy of two clusters
double h(std::map<long, bool>& c1, std::map<long, bool>& c2, long inter, long n) {
  long s1 = c1.size(), s2 = c2.size();
  double in = inter;
  double un = s1 + s2 - inter;
  double sum = h(in/n);
  sum += h((s1 - in)/n);
  sum += h((s2 - in)/n);
  sum += h((n - un)/n);
  return sum;
}

bool satisfy_lancichinetti_constraint(std::map<long, bool>& c1, std::map<long, bool>& c2, long inter, long n) {
  long s1 = c1.size(), s2 = c2.size();
  double in = inter;
  double un = s1 + s2 - inter;
  double sum1 = h(in/n) + h((n - un)/n);
  double sum2 = h((s1 - in)/n) + h((s2 - in)/n);
  return sum1 > sum2;
}


double NmiCM::apply(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2) {
  std::vector<std::map<long, bool> > clust1, clust2;
  unsigned long n = memb1.size();
  membership_to_clusters(memb1, clust1);
  membership_to_clusters(memb2, clust2);
  
	std::map<std::string, long> intersections;
	//We store the clusters whose size is over 1/2 the network since they may satisfy
  //lancichinetti constraint with another cluster even with a null intersection
  double thre = ((double)n)/2;
  for (long i = 0; i < clust1.size(); i++) {
    if (clust1[i].size() > thre) {
      for (long j = 0; j < clust2.size(); j++) {
				intersections[to_string(i,j)] = intersection(clust1[i], clust2[j]);
				//std::cout << "Inter c1 " << i << " c2 " << j << " : " << intersections[to_string(i,j)];
      }
      
    }
  }
  for (long j = 0; j < clust2.size(); j++) {
    if (clust2[j].size() > thre) {
      for (long i = 0; i < clust1.size(); i++)
				if(intersections.find(to_string(i,j)) == intersections.end()) {
					intersections[to_string(i,j)] = intersection(clust1[i], clust2[j]);
					//std::cout << "Inter c1 " << i << " c2 " << j << " : " << intersections[to_string(i,j)];
				}
		}
  }

  //We now store the real intersections
  for(unsigned long i = 0; i < memb1.size(); i++) {
    std::list<long> l1 = memb1[i];
    std::list<long> l2 = memb2[i];
    for(std::list<long>::iterator iter = l1.begin(); iter != l1.end(); iter++) {
      for(std::list<long>::iterator iter2 = l2.begin(); iter2 != l2.end(); iter2++) {
				long c1 = *iter;
				long c2 = *iter2;
				std::string id = to_string(c1, c2);
				if(intersections.find(id) == intersections.end()) {
					intersections[id] = intersection(clust1[c1], clust2[c2]);
					//std::cout << "Inter c1 " << c1 << " c2 " << c2 << " : " << intersections[id] << std::endl;
				}
				if(intersections[id] == 0) {
					std::cerr << "Problem intersection between two cluster is 0, this should not happen" << std::endl;
					exit(1);
				}
      }	
    }
  }
  
  //Now we find the entropy minimum between X_k and all relevant Y_l, and inversly
  std::map<long, double> hmin1, hmin2;
  for(std::map<std::string, long>::iterator iter = intersections.begin(); iter != intersections.end(); iter++) {
    std::pair<long, long> p = from_string(iter->first);
    long c1 = p.first;
    long c2 = p.second;
    long inter = iter->second;
    
    if(satisfy_lancichinetti_constraint(clust1[c1], clust2[c2], inter, n)) {
      double h_X_Y = h(clust1[c1], clust2[c2], inter, n) - h(clust2[c2], n);
      if(hmin1.find(c1) == hmin1.end() || hmin1[c1] > h_X_Y)
				hmin1[c1] = h_X_Y;
      double h_Y_X = h(clust1[c1], clust2[c2], inter, n) - h(clust1[c1], n);
      if(hmin2.find(c2) == hmin2.end() || hmin2[c2] > h_Y_X)
				hmin2[c2] = h_Y_X;
      //std::cout << "Intersection (satisfying) " << c1 << " " << c2 << " " << inter  << " " << h_X_Y << " " << h_Y_X << std::endl;
    }
  }
  
  //We now regroup in H(X|Y)_norm and H(Y|X)_norm 
  double H_X_Y = 0, H_Y_X = 0;
  long len1 = 0, len2 = 0;
  for (long i = 0; i < clust1.size(); i++) {
    if(clust1[i].size() > 0) {
      len1++;
      if(hmin1.find(i) == hmin1.end()) {
				H_X_Y += 1;
      }
      else {
				//std::cout << "X|Y i " << i << "hmin " << hmin1[i]/h(clust1[i], n) << std::endl;
				if(h(clust1[i], n) > 0)
					H_X_Y += hmin1[i]/h(clust1[i], n);    
      }
    }
  }
  for (long i = 0; i < clust2.size(); i++) {
    if(clust2[i].size() > 0) {
      len2++;
      if(hmin2.find(i) == hmin2.end())
				H_Y_X += 1;
      else {
				//std::cout << "Y|X i " << i << " hmin " << hmin2[i] << " h " << h(clust2[i], n) << std::endl;
				if(h(clust2[i], n) > 0)
					H_Y_X += hmin2[i]/h(clust2[i], n);
      }
    }
  }
  
  //std::cout << H_X_Y/len1 << " " << len1 << " " << H_Y_X/len2 << " " <<  len2 << std::endl;
  return 1 - (H_X_Y/len1 + H_Y_X/len2)/2;
}
