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
#include "SignificanceQF.hpp"
#include <cmath>
#include <map>
#include <iostream>

SignificanceQF::SignificanceQF() {
  name = "sign";
  description = "Signifiance, from Traag et al., 'Significant Scales in Community Structure', 2013";
}

SignificanceQF::~SignificanceQF() {
}

double SignificanceQF::apply(igraph_t* graph, std::vector<long> & membership) {
  unsigned long n = igraph_vcount(graph);
  unsigned long m = igraph_ecount(graph);
  
  unsigned long m_in = 0;
  std::map<long, long> c_m_in;
  igraph_eit_t eit;
  igraph_eit_create(graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);
  while(!IGRAPH_EIT_END(eit)) {
    igraph_integer_t eid = IGRAPH_EIT_GET(eit), from, to;
    igraph_edge(graph, eid, &from, &to);
    if(membership[from] == membership[to]) {
      long c_id = membership[from];
      m_in++;
      if(c_m_in.find(c_id) == c_m_in.end())
	c_m_in[c_id] = 1;
      else
	c_m_in[c_id]++;
    }
    IGRAPH_EIT_NEXT(eit);
  }
  igraph_eit_destroy(&eit);
  
  std::map<long, long> c_size;
  for(long i = 0; i < membership.size(); i++) {
    long c_id = membership[i];
    if(c_size.find(c_id) == c_size.end())
      c_size[c_id] = 1;
    else
      c_size[c_id]++;
  }

  unsigned long M_in = 0;
  double sum = 0;
  double p = 2*((double)m)/(n*(n - 1));
  for(std::map<long, long>::iterator iter = c_size.begin(); iter != c_size.end(); iter++) {
    //std::cout << "start" << std::endl;
    long c_id = iter->first;
    long n_c = iter->second;
    long m_c = 0;
    if(c_m_in.find(c_id) != c_m_in.end())
      m_c = c_m_in[c_id];
    double p_c = 2*((double)m_c)/(n_c*(n_c - 1));
    //std::cout << m_c << " " << p_c << " " << p << std::endl;
    double temp = 0;
    if (p_c > 0)
      temp += p_c*log(p_c/p);
    if(p_c < 1)
      temp += (1 - p_c)*log((1 - p_c)/(1 - p));
    sum += (n_c*(((double)n_c) - 1)/2)*temp;
  }

  return sum;
}

double SignificanceQF::single_apply(igraph_t* graph, std::vector<long> & membership, std::deque<long> & nodes) {
  return -1;
}

