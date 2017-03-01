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
#include "FBCubedCM.hpp"
#include <map>
#include <cstdlib>
#include <sstream>
#include "sets.hpp"

//The contribution of an individual to the bcubed precision and recall
std::pair<double, double> bcubed_individual(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2, std::vector<std::map<long, bool> >& clust1, std::vector<std::map<long, bool> >& clust2, long v);

FBCubedCM::FBCubedCM() {
  name = "fbcubed";
  std::stringstream s;
  s << std::dec << NB_SAMPLES;
  description = "The overlapping F-BCubed measure. See Amigo et al., 'A comparison of Extrinsic Clustering Evaluation Metrics based on Formal Constraints', 2009. Warning : the method uses " + s.str() + " random samples if the number of elements is bigger";
}

FBCubedCM::~FBCubedCM() {
}

double FBCubedCM::apply(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2) {
  std::pair<double, double> b3 = bcubed_sampling(memb1, memb2, NB_SAMPLES);
  double prec = b3.first;
  double rec = b3.second;
  return 1/(1/(2*prec) + 1/(2*rec));
}

std::pair<double, double> FBCubedCM::bcubed_sampling(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2, long nb_samples) {
  std::pair<double, double> res, final_result;
  final_result.first = 0;
  final_result.second = 0;

  std::vector<std::map<long, bool> > clust1, clust2;
  membership_to_clusters(memb1, clust1);
  membership_to_clusters(memb2, clust2);

  bool random = true;
  if(nb_samples > memb1.size()) { //If the number of samples is over the size of the graph, a random sampling is useless
    random = false;
    nb_samples = memb1.size();
  }

  for(long i = 0; i < nb_samples; i++) {
    long v;
    if(random)
      v = rand() % memb1.size();
    else
      v = i;
    res = bcubed_individual(memb1, memb2, clust1, clust2, v);
    final_result.first *= ((double)i)/(i+1);
    final_result.first += res.first/(i+1);

    final_result.second *= ((double)i)/(i+1);
    final_result.second += res.second/(i+1);
    // double running = 0;
    // if(final_result.first > 0 && final_result.second > 0)
    //   running = 1.0/(1.0/(2.0*final_result.first) + 1.0/(2.0*final_result.second));
    // std::cerr << i << " " << res.first << " " << res.second;
    // std::cerr << " " << final_result.first;
    // std::cerr << " " << final_result.second;
    // std::cerr << " " << running << std::endl;
  }
  
  return final_result;
}


std::pair<double, double> bcubed_individual(std::vector<std::list<long> >& memb1, std::vector<std::list<long> >& memb2, std::vector<std::map<long, bool> >& clust1, std::vector<std::map<long, bool> >& clust2, long v) {
  std::pair<double, double> result;
  std::map<long, long> inter1, inter2;
  
  for(std::list<long>::iterator it_memb1 = memb1[v].begin(); it_memb1 != memb1[v].end(); it_memb1++) {
    long c1 = *it_memb1;
    for(std::map<long, bool>::iterator it_c1 = clust1[c1].begin(); it_c1 != clust1[c1].end(); it_c1++) {
      long u = it_c1->first;
      if(inter1.find(u) == inter1.end())
	inter1[u] = 1;
      else
	inter1[u] += 1;
    }
  }

  for(std::list<long>::iterator it_memb2 = memb2[v].begin(); it_memb2 != memb2[v].end(); it_memb2++) {
    long c2 = *it_memb2;
    for(std::map<long, bool>::iterator it_c2 = clust2[c2].begin(); it_c2 != clust2[c2].end(); it_c2++) {
      long u = it_c2->first;
      if(inter2.find(u) == inter2.end())
	inter2[u] = 1;
      else
	inter2[u] += 1;
    }
  }
  
  double sum = 0.0;
  for(std::map<long, long>::iterator it_inter1 = inter1.begin(); it_inter1 != inter1.end(); it_inter1++) {
    long u = it_inter1->first;
    if(inter2.find(u) != inter2.end()) {
      if(inter2[u] < inter1[u])
	sum += ((double)inter2[u])/inter1[u];
      else
	sum += 1;
    }
  }
  if(inter1.size() > 0)
    result.first = sum/inter1.size();
  else
    result.first = 0;

  sum = 0.0;
  for(std::map<long, long>::iterator it_inter2 = inter2.begin(); it_inter2 != inter2.end(); it_inter2++) {
    long u = it_inter2->first;
    if(inter1.find(u) != inter1.end()) {
      if(inter1[u] < inter2[u])
	sum += ((double)inter1[u])/inter2[u];
      else
	sum += 1;
    }
  }
  if(inter2.size() > 0)
    result.second = sum/inter2.size();
  else
    result.second = 0;

  return result;
}
