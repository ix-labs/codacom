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

#include "CMProvider.hpp"

#include "file_operations.hpp"

void usage(std::string more, CMProvider & provider) {
  std::cerr << more << std::endl;
  std::cerr << "usage: [-p input_prefix] [-c method]* input_dir outfile" << std::endl << std::endl;
  std::cerr << "Compares the different membership files with the input metrics" << std::endl;
  std::cerr << "input_dir\tDirectory containing membership files (each line is : node_id community_id)" << std::endl;
  std::cerr << "output_file\tThe file containing the result (each line/column is a membership file, each value is the correlation between the clusterings)" << std::endl;
  std::cerr << "-p input_prefix\tThe compared files have a name featuring this prefix" << std::endl;
  std::cerr << "-c metric\tComparison method used to compare the input files" << std::endl;
  
  std::list<AbstractCM*> all_CM;
  provider.getAllCM(all_CM);
  for(std::list<AbstractCM*>::iterator it_cm = all_CM.begin(); it_cm != all_CM.end(); it_cm++)
    std::cerr << "\t" << (*it_cm)->getName() << " : " << (*it_cm)->getDescription() << std::endl;;

  exit(1);
}

std::string indir_s = "";
std::string outfile_s = "";
std::string in_prefix = "";

void parse_args(int argc, char **argv, CMProvider & provider) {
  opterr = 0;
  int c, i;
  while ((c = getopt (argc, argv, "p:c:")) != -1) {
    switch (c) {
    case 'p':
      in_prefix = optarg;
      break;
    case 'c': {
      std::string s(optarg);
      int err = provider.setCMAsAvailable(s);
      if(err) {
	usage("Comparison method " + s + "  not recognised.", provider);
      }
      break;
    } case '?':
      if (optopt == 'c')
	usage("Option -%c requires an argument", provider);
      else if (isprint (optopt))
	usage("Unknown option `-%c'", provider);
      else
	usage("Unknown option character `\\x%x'", provider);
    default:
      abort ();
    }
  }
  
  indir_s = argv[optind];
  outfile_s = argv[optind+1];
}

void vector_to_igraph(std::vector<long>& in, igraph_vector_t* out) {
  igraph_vector_resize(out, in.size());
  for(unsigned long i = 0; i < in.size(); i++)
    igraph_vector_set(out, i, in[i]);
}

int main(int argc, char* argv[])
{
  CMProvider provider;
  parse_args(argc, argv, provider);
  if(indir_s[indir_s.size()-1] != '/') //The dir string always ends with a /
    indir_s += '/';

  std::list<std::string> files;
  int err = list_files(indir_s, in_prefix, files);
  if(err == 1)
    usage("Could not open directory "+indir_s, provider);

  std::vector<std::list<long> > base_memb, other_memb;

  std::list<AbstractCM*> all_available_CM;
  provider.getAllAvailableCM(all_available_CM);
  for(std::list<AbstractCM*>::iterator it_cm = all_available_CM.begin(); it_cm != all_available_CM.end(); it_cm++) {  
    std::ofstream outfile;
    outfile.open((outfile_s + "_" + (*it_cm)->getName()).c_str(), std::ofstream::out | std::ofstream::trunc);
  
    outfile << in_prefix << " ";
    std::vector<std::string> titles;
    for(std::list<std::string>::iterator iter = files.begin(); iter!=files.end(); iter++) {
      std::string title = (*iter).substr(indir_s.size());
      title.erase(0, in_prefix.size());
      titles.push_back(title);
      outfile << title << " ";
    }
    outfile << std::endl;

    int j = 0;
    for(std::list<std::string>::iterator iter = files.begin(); iter!=files.end(); iter++) {
      read_membership((*iter).c_str(), base_memb);
      outfile << titles[j] << " ";
      for(std::list<std::string>::iterator iter2 = files.begin(); iter2!=files.end(); iter2++) {
	if(iter != iter2) {
	  read_membership((*iter2).c_str(), other_memb);
	  igraph_real_t result = 0.0;
	  result = (*it_cm)->apply(base_memb, other_memb);
	  outfile << result << " ";
	} else {
	  outfile << 1 << " ";
	}
      }
      outfile << std::endl;
      j++;
    }
    outfile << std::endl;
    outfile.close();
  }
}
