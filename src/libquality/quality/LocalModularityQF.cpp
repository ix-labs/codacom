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
#include "LocalModularityQF.hpp"
#include <map>
#include <iostream>

LocalModularityQF::LocalModularityQF() {
  name = "local_modularity";
  description = "The local modularity, from Muff et al., 'Local modularity measure for network clusterizations', 2005";
}

LocalModularityQF::~LocalModularityQF() {
}

double LocalModularityQF::apply(igraph_t* graph, std::vector<long> & membership) {
	double res = 0.0;

	//Computing the internal edges of all clusters
	std::map<long, long> inter_clust;

	//Computing the cluster neighbors of each cluster
	std::map<long, std::map<long, long> > neighbors;
	igraph_vit_t vit;
  igraph_vit_create(graph, igraph_vss_all(), &vit);
	igraph_vector_t neis;
	igraph_vector_init(&neis, 0);
  while (!IGRAPH_VIT_END(vit)) {
    long id = IGRAPH_VIT_GET(vit);
		long c1 = membership[id];
		igraph_neighbors(graph, &neis, id, IGRAPH_ALL);
		for(long i = 0; i < igraph_vector_size(&neis); i++) {
			long id2 = igraph_vector_e(&neis, i);
			long c2 = membership[id2];
			if(c1 != c2) {
				if(neighbors.find(c1) == neighbors.end()) {
					neighbors[c1] = std::map<long, long>();
				}
				
				if(neighbors[c1].find(c2) == neighbors[c1].end())
					neighbors[c1][c2] = 0;
				neighbors[c1][c2]++;
			} else {
				if(inter_clust.find(c1) == inter_clust.end())
					inter_clust[c1] = 0;
				if(id > id2) //Avoid duplicates
					inter_clust[c1]++;
			}
		}
    IGRAPH_VIT_NEXT(vit);
  }
  igraph_vit_destroy(&vit);
	igraph_vector_destroy(&neis);

	//Computing, for each community, the local modularity
	for(std::map<long, std::map<long, long> >::iterator iter = neighbors.begin(); iter != neighbors.end(); iter++) {
		//Edges in the neighborhood
		long c = iter->first;
		long size = inter_clust[c];
		long vol = 2*size;
		long lin = size;
		for(std::map<long, long>::iterator n_iter = iter->second.begin(); n_iter != iter->second.end(); n_iter++) {
			lin += n_iter->second;
			vol += n_iter->second;
			lin += inter_clust[n_iter->first];
		}
		
		//std::cout << "c : " << c << " size " << size << " lin " << lin << " loc_mod : " << ((double)size)/lin - ((double)size*size)/(lin*lin) << std::endl;
		res += ((double)size)/lin - ((double)vol*vol)/(4*lin*lin);
	}
  return res;
}

double LocalModularityQF::single_apply(igraph_t* graph, std::vector<long> & membership, std::deque<long> & nodes) {
  return -1;
}
