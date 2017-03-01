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

#include <file_operations.hpp>
#include "QFProvider.hpp"
#include "CMProvider.hpp"

void usage(const char *more, QFProvider& qf_provider, CMProvider& cm_provider) {
  std::cerr << more << std::endl;
  std::cerr << "usage: -g input_graph -i input_dir -o outfile [-t ground_truth [-c comparison]] [-p input_prefix] [-q function]* [-e function]*" << std::endl << std::endl;
  std::cerr << "Gives the quality of each input clustering" << std::endl;
  std::cerr << "-i input_dir\tDirectory containing membership files (each line is : node_id community_id)" << std::endl;
  std::cerr << "-g input_graph\tGraph on which the clusterings have been produced (edge list)" << std::endl;
  std::cerr << "-o output_file\tThe file containing the result (each column is a quality function, each line is a membership file" << std::endl;
  std::cerr << "-t ground_truth\tGround-truth membership file" << std::endl;
  std::cerr << "-c comparison\tComparison method used to compare clusterings et ground-truth. Choices are : " << std::endl;
  std::cerr << "\tall : all the comparison functions will be applied" << std::endl;
  std::list<AbstractCM*> all_CM;
  cm_provider.getAllCM(all_CM);
  for(std::list<AbstractCM*>::iterator it_cm = all_CM.begin(); it_cm != all_CM.end(); it_cm++)
    std::cerr << "\t" << (*it_cm)->getName() << " : " << (*it_cm)->getDescription() << std::endl;;

  std::cerr << "-p input_prefix\tThe compared files have a name featuring this prefix" << std::endl;
  std::cerr << "-q function\tFunction used to asses the quality of the clusterings. Choices are : " << std::endl;
  std::cerr << "\tall : all the quality functions will be applied" << std::endl;
  std::list<AbstractQF*> all_QF;
  qf_provider.getAllQF(all_QF);
  for(std::list<AbstractQF*>::iterator it_qf = all_QF.begin(); it_qf != all_QF.end(); it_qf++)
    std::cerr << "\t" << (*it_qf)->getName() << " : " << (*it_qf)->getDescription() << std::endl;;
  std::cerr << "-e function\tMutually exclusive with -q. Run all quality functions except those indicated. Choices are the same as -q" << std::endl;
  exit(1);
}

void usage(std::string more, QFProvider& qf_provider, CMProvider& cm_provider) {
  usage(more.c_str(), qf_provider, cm_provider);
}


std::string indir_s = "";
std::string ingraph_s = "";
std::string outfile_s = "";
std::string in_prefix = "";
std::string ground_file_s = "";

void parse_args(int argc, char **argv, QFProvider& qf_provider, CMProvider& cm_provider) {
  bool q=false;
  bool e=false;
  for(int i = 1; i < argc; i++) {
    int err;
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'i':
				if(i==argc-1)
					usage("Indir missing", qf_provider, cm_provider);
				indir_s = argv[i+1];
				i++;
				break;
      case 'g':
				if(i==argc-1)
					usage("Input graph missing", qf_provider, cm_provider);
				ingraph_s = argv[i+1];
				i++;
				break;
      case 'o':
				if(i==argc-1)
					usage("Outfile missing", qf_provider, cm_provider);
        outfile_s = argv[i+1];
				i++;
				break;
      case 't':
				if(i==argc-1)
					usage("Ground truth file missing", qf_provider, cm_provider);
        ground_file_s = argv[i+1];
				i++;
				break;
      case 'p':
				if (i==argc-1)
					usage("Prefix missing", qf_provider, cm_provider);
        in_prefix = argv[i+1];
				i++;
				break;
      case 'c' :
        if(i==argc-1)
					usage("Comparison method missing", qf_provider, cm_provider);
				if(std::string(argv[i+1]) == "all")
					err = cm_provider.setAllCMAsAvailable();
				else
					err = cm_provider.setCMAsAvailable(argv[i+1]);
				if(err != 0)
					usage("Comparison method not recognized : "+std::string(argv[i+1]), qf_provider, cm_provider);
				q = true;
				i++;
				break;
      case 'q' :
        if(i==argc-1)
					usage("Quality function missing", qf_provider, cm_provider);
				if(e)
					usage("-q and -e are mutually exclusive", qf_provider, cm_provider);
				if(std::string(argv[i+1]) == "all")
					qf_provider.setAllQFAsAvailable();
				else
					err = qf_provider.setQFAsAvailable(argv[i+1]);
				if(err != 0)
					usage("Quality function not recognized : "+std::string(argv[i+1]), qf_provider, cm_provider);
				q = true;
				i++;
				break;
      case 'e' :
				if(i==argc-1)
					usage("Quality function missing", qf_provider, cm_provider);
				if(q)
					usage("-q and -e are mutually exclusive", qf_provider, cm_provider);
				if(!e) {
					qf_provider.setAllQFAsAvailable();
				}
				err = qf_provider.setQFAsUnavailable(argv[i+1]);
				if(err != 0)
					usage("Quality function not recognized : "+std::string(argv[i+1]), qf_provider, cm_provider);
				e = true;
				i++;
				break;
      default:
				usage("Unknown option", qf_provider, cm_provider);
      }
    } else {
      usage("More than one filename", qf_provider, cm_provider);
    }
  }
  if (indir_s=="" || outfile_s=="" || ingraph_s=="")
    usage("Indir, input graph or outfile missing", qf_provider, cm_provider);
}

int main(int argc, char* argv[])
{
  QFProvider qf_provider;
  CMProvider cm_provider;
  parse_args(argc, argv, qf_provider, cm_provider);

  //Initialisation de la seed random au temps
  srand(time(NULL));

	//std::cout << indir_s << " " << ingraph_s << " " << outfile_s << " " << in_prefix << " " << ground_file_s << std::endl;
  if(indir_s[indir_s.size()-1] != '/') //The dir string always ends with a /
    indir_s += '/';

  igraph_t graph;
  FILE* in = fopen(ingraph_s.c_str(), "r");
  igraph_read_graph_edgelist(&graph, in, 0, false);
  fclose(in);
  
  bool with_gt = false;
  std::vector<std::list<long> > memb_gt;
  if(ground_file_s != "") {
    with_gt = true;
    read_membership(ground_file_s, memb_gt);
  }

  std::list<std::string> files;
  int err = list_files(indir_s, in_prefix, files);
  if(err == 1)
    usage("Could not open directory", qf_provider, cm_provider);

  std::ofstream outfile;
  outfile.open(outfile_s.c_str(), std::ofstream::out | std::ofstream::trunc);

  outfile << "method\\quality";

  std::list<AbstractCM*> all_available_CM;
  cm_provider.getAllAvailableCM(all_available_CM);
  for(std::list<AbstractCM*>::iterator it_cm = all_available_CM.begin(); it_cm != all_available_CM.end(); it_cm++)
    outfile << " " << (*it_cm)->getName() << "_gt";

  std::list<AbstractQF*> all_available_QF;
  qf_provider.getAllAvailableQF(all_available_QF);
  for(std::list<AbstractQF*>::iterator it_qf = all_available_QF.begin(); it_qf != all_available_QF.end(); it_qf++)
    outfile << " " << (*it_qf)->getName();
  outfile << std::endl;

  std::vector<std::list<long> > memb;
  for(std::list<std::string>::iterator iter = files.begin(); iter!=files.end(); iter++) {
    std::string title = (*iter).substr(indir_s.size());
    title.erase(0, in_prefix.size());
    outfile << title;
		//std::cout << title << std::endl;
    
    memb.erase(memb.begin(), memb.end());
    read_membership((*iter).c_str(), memb);
		
    if(with_gt) {
      for(std::list<AbstractCM*>::iterator it_cm = all_available_CM.begin(); it_cm != all_available_CM.end(); it_cm++) {
				//std::cout << (*it_cm)->getName() << std::endl;
				outfile << " " << (*it_cm)->apply(memb_gt, memb);
      }
    }
    
    for(std::list<AbstractQF*>::iterator it_qf = all_available_QF.begin(); it_qf != all_available_QF.end(); it_qf++) {
      std::vector<long> memb_temp(memb.size());
      bool warning = false;
      for(unsigned long i = 0; i<memb.size(); i++) {
	if(memb[i].size() > 1)
	  warning = true;
	memb_temp[i] = memb[i].front();
      }
      if(warning) {
	std::cout << "Warning : trying to apply non-overlapping quality function " << (*it_qf)->getName() <<" on overlapping membership " << title << std::endl;
      }

      //std::cout << "Applying qf " << (*it_qf)->getName() << std::endl;
      outfile << " " << (*it_qf)->apply(&graph, memb_temp);
    }
    outfile << std::endl;
    // for(long i = 0; i<memb.size(); i++)
    //   delete &(memb[i]);
  }
  igraph_destroy(&graph);
}
