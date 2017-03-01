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
#include "ClustCoefSCQF.hpp"
#include <iostream>

ClustCoefSCQF::ClustCoefSCQF() {
  name = "cc";
  description = "the internal clustering coefficient";
}

double ClustCoefSCQF::apply(igraph_t* graph, std::vector<long> & membership, std::deque<long> & nodes) {
  double res = 1;
  unsigned long nb_nodes = 0;
  igraph_vector_t neis;
  igraph_vector_init(&neis, 0);
  for(std::deque<long>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
    igraph_integer_t n0 = *iter;
    igraph_neighbors(graph, &neis, n0, IGRAPH_ALL);

    unsigned long nb_tr = 0;
    unsigned long internal_degree = 0;
    for(long i = 0; i < igraph_vector_size(&neis); i++) {
      igraph_integer_t n1 = igraph_vector_e(&neis, i);
      if(membership[n0] == membership[n1]) { //n1 needs to be in the same cluster as n0
				internal_degree++;
				for(long j = i+1; j < igraph_vector_size(&neis); j++) {
					igraph_integer_t n2 = igraph_vector_e(&neis, j);
					if(membership[n0] == membership[n2]) { //n2 needs to be in the same cluster as n0
						igraph_integer_t eid;
						igraph_get_eid(graph, &eid, n1, n2, 0, 0);
						if(eid > -1) //If the edge has been found, there is a triangle
							nb_tr++;
					}
				}
      }
    }
    nb_nodes++;
    double clu_co = 0;
    if(internal_degree > 1)
      clu_co = 2*((double)nb_tr)/(internal_degree*(internal_degree-1));
    res = res*(nb_nodes - 1)/((double)nb_nodes) + clu_co/((double)nb_nodes);
  }
  igraph_vector_destroy(&neis);
  return res;
}
