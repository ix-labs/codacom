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
#include "algorithms.h"
#include <iostream>

#include <config.h>
#if LEAD_EIG_ELEVEN_ARGS
#   define igraph_community_leading_eigenvector(a,b,c,d,e,f,g,h,i,j,k,l,m) igraph_community_leading_eigenvector(a,c,d,e,f,g,h,i,j,k,l,m)
#endif

void clauset(igraph_t* graph, igraph_vector_t* membership) {  
  igraph_vector_t modularity;
  igraph_matrix_t merges;
  igraph_vector_init(&modularity,0);
  igraph_matrix_init(&merges,0,0);
  igraph_community_fastgreedy(graph, NULL, &merges, &modularity, membership);
  //Prints the dendogram
  if(true) {
    for(long i = 0; i < igraph_matrix_nrow(&merges); i++) {
      std::cout << igraph_matrix_e(&merges, i, 0) << " " << igraph_matrix_e(&merges, i, 1) << std::endl;
    }
  }
  igraph_matrix_destroy(&merges);
  igraph_vector_destroy(&modularity);
}

void spinglass(igraph_t* graph, igraph_vector_t* membership) {
  igraph_community_spinglass(graph, 
			     0, /* no weights */
			     NULL,
			     NULL,
			     membership,
			     NULL,
			     2,	   /* no of spins */
			     0,    /* parallel update */
			     1.0,  /* start temperature */
			     0.01, /* stop temperature */
			     0.99, /* cooling factor */
			     IGRAPH_SPINCOMM_UPDATE_CONFIG,
			     1.0, /* gamma */
			     IGRAPH_SPINCOMM_IMP_ORIG,
			     /*gamma-=*/ 0);
}

void leading_eigenvector(igraph_t* graph, igraph_vector_t* membership) {
  igraph_arpack_options_t options;
  igraph_matrix_t merges;
  igraph_arpack_options_init(&options);
  igraph_matrix_init(&merges,0,0);
  igraph_community_leading_eigenvector(graph, /*weights=*/ 0, &merges, 
				       membership, igraph_vcount(graph), 
				       &options, /*modularity=*/ 0, 
				       /*start=*/ 0, /*eigenvalues=*/ 0, 
				       /*eigenvectors=*/ 0, /*history=*/ 0,
				       /*callback=*/ 0, 
				       /*callback_extra=*/ 0);
   igraph_matrix_destroy(&merges);
}

void walktrap(igraph_t* graph, igraph_vector_t* membership) {
  igraph_vector_t modularity;
  igraph_matrix_t merges;
  igraph_vector_init(&modularity,0);
  igraph_matrix_init(&merges,0,0);
  igraph_community_walktrap(graph, 0 /* no weights */,
			    4 /* steps */,
			    &merges, &modularity, 
			    /* membership=*/ membership);
  igraph_matrix_destroy(&merges);
  igraph_vector_destroy(&modularity);
}

void betweenness(igraph_t* graph, igraph_vector_t* membership) {
  igraph_vector_t modularity;
  igraph_matrix_t merges;
  igraph_vector_init(&modularity,0);
  igraph_matrix_init(&merges,0,0);
  igraph_community_edge_betweenness(graph, NULL, NULL, 0 /*merges */,
				    0 /*bridges */, /*modularity=*/ &modularity,
				    /*membership=*/ membership,
				    IGRAPH_UNDIRECTED,
				    /*weights=*/ 0);
  igraph_matrix_destroy(&merges);
  igraph_vector_destroy(&modularity);
}

void label_prop(igraph_t* graph, igraph_vector_t* membership) {
  igraph_community_label_propagation(graph, membership, 0, 0, 0, 
				     /*modularity=*/ 0);
}

void infomap(igraph_t* graph, igraph_vector_t* membership) {
  igraph_real_t codelength = 1000;
  igraph_community_infomap(graph, NULL, NULL, 1, membership, &codelength);
}
