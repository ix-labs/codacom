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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <fstream>
#include <list>
#include <deque>
#include <map>

#include <file_operations.hpp>
#include "QFProvider.hpp"

void usage(const char *more, QFProvider& provider) {
  std::cerr << more << std::endl;
  std::cerr << "usage: [-q function]* [-e function]* input_graph input_clust outdir" << std::endl << std::endl;
  std::cerr << "Output three files in the outdir : quality (the quality of each cluster considereing different metrics), size_distrib (the distribution of cluster size) and graph_report (various statistics on the graph)." << std::endl;
  std::cerr << "input_clust\tThe membership file (each line is : node_id community_id)" << std::endl;
  std::cerr << "input_graph\tGraph on which the clusterings have been produced (edge list)" << std::endl;
  std::cerr << "outdir\tDirectory where the reports are stored" << std::endl;
  std::cerr << "-o output_dir\tThe directory where results files will be generated" << std::endl;
  std::cerr << "-q function\tFunction used to asses the quality of the clusterings. Choices are : " << std::endl;
  std::cerr << "\tall : all the quality functions will be applied" << std::endl;;
  std::list<AbstractQF*> all_QF;
  provider.getAllQF(all_QF);
  for(std::list<AbstractQF*>::iterator it_qf = all_QF.begin(); it_qf != all_QF.end(); it_qf++)
    std::cerr << "\t" << (*it_qf)->getName() << " : " << (*it_qf)->getDescription() << std::endl;;
  std::cerr << "-e function\tMutually exclusive with -q. Run all quality functions except those indicated. Choices are the same as -q" << std::endl;
  exit(1);
}

void usage(std::string more, QFProvider& provider) {
  usage(more.c_str(), provider);
}


std::string clust_s = "";
std::string ingraph_s = "";
std::string outdir_s = "";

void parse_args(int argc, char **argv, QFProvider& provider) {
  bool q = false;
  bool e = false;
  int i = 1;
  while(i < argc && argv[i][0] == '-') {
    int err;    
    switch(argv[i][1]) {
    case 'q' :
      if(i==argc - 1)
				usage("Quality function missing", provider);
      if(e)
				usage("-q and -e are mutually exclusive", provider);
      if(std::string(argv[i+1]) == "all")
				provider.setAllQFAsAvailable();
      else
				err = provider.setQFAsAvailable(argv[i+1]);
      if(err != 0)
				usage("Quality function not recognized : "+std::string(argv[i+1]), provider);
      q = true;
      i++;
      break;
    case 'e' :
      if(i == argc-1)
				usage("Quality function missing", provider);
      if(q)
				usage("-q and -e are mutually exclusive", provider);
      if(!e) {
				provider.setAllQFAsAvailable();
      }
      err = provider.setQFAsUnavailable(argv[i+1]);
      if(err != 0)
				usage("Quality function not recognized : "+std::string(argv[i+1]), provider);
      e = true;
      i++;
      break;
    default:
      usage("Unknown option", provider);
    }
    i++;
  }
  if(argc - i < 3)
    usage("Not enough arguments", provider);
  ingraph_s = argv[i];
  clust_s = argv[i+1];
  outdir_s = argv[i+2];
  if(outdir_s[outdir_s.size() - 1] != '/')
    outdir_s += '/';
}

int main(int argc, char* argv[])
{
  QFProvider provider;
  parse_args(argc, argv, provider);

  igraph_t graph;
  FILE* in = fopen(ingraph_s.c_str(), "r");
  igraph_read_graph_edgelist(&graph, in, 0, false);
  fclose(in);

  //Convert membership to list of communities
  std::vector<long> membership;
  read_membership(clust_s.c_str(), membership);
  long max_membership = 0;
  for(std::vector<long>::iterator iter = membership.begin(); iter != membership.end(); iter++) {
    if(*iter > max_membership)
      max_membership = *iter;
  }
  std::vector<std::deque<long> > communities(max_membership+1);
  long i = 0;
  for(std::vector<long>::iterator iter = membership.begin(); iter != membership.end(); iter++) {
    communities[*iter].push_back(i);
    i++;
  }

  //Creation of the quality file, storing for each cluster its qualities
  std::ofstream out_q;
  out_q.open((outdir_s + "quality").c_str(), std::ofstream::out | std::ofstream::trunc);

  out_q << "cluster\\quality size ";
  std::list<AbstractQF*> all_available_QF;
  provider.getAllAvailableQF(all_available_QF);
  for(std::list<AbstractQF*>::iterator it_qf = all_available_QF.begin(); it_qf != all_available_QF.end(); it_qf++)
    out_q << (*it_qf)->getName() << " ";
  out_q << std::endl;

  i = 0;
  for(std::vector<std::deque<long> >::iterator iter = communities.begin(); iter!=communities.end(); iter++) {
    if(iter->size() > 0) {
      out_q << i << " " << iter->size() << " ";

      for(std::list<AbstractQF*>::iterator it_qf = all_available_QF.begin(); it_qf != all_available_QF.end(); it_qf++) {
				out_q << (*it_qf)->single_apply(&graph, membership, *iter) << " ";
      }

      out_q << std::endl;
      i++;
    }
  }
  out_q.close();

  //Creating the size distribution file, for each size there is the cardinal of clusters of this size
  std::map<long, long> size_to_cardinal; //The map that stores the distributions
  for(std::vector<std::deque<long> >::iterator iter = communities.begin(); iter!=communities.end(); iter++) {
    long size = iter->size();
    if(size > 0) {
      if(size_to_cardinal.find(size) == size_to_cardinal.end())
				size_to_cardinal[size] = 0;
      size_to_cardinal[size] += 1;
    }
  }

  std::ofstream out_size;
  out_size.open((outdir_s + "size_distrib").c_str(), std::ofstream::out | std::ofstream::trunc);
  for(std::map<long, long>::iterator iter = size_to_cardinal.begin(); iter != size_to_cardinal.end(); iter++) {
    out_size << iter->first << " " << iter->second << std::endl;
  }
  out_size.close();

  //Creating a report on the graph itself
  std::ofstream out_rep;
  out_rep.open((outdir_s + "graph_report").c_str(), std::ofstream::out | std::ofstream::trunc);
  out_rep << "Nodes : " << igraph_vcount(&graph) << ", edges : " << igraph_ecount(&graph);
  out_rep.close();

  igraph_destroy(&graph);
}
