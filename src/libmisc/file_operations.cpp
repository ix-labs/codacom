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
#include "file_operations.hpp"
#include <cstring>
#include <sstream>
#include <algorithm> 

void read_membership(std::string filename, std::vector<long> & membership) {
  std::ifstream infile;
  infile.open(filename.c_str(), std::ifstream::in);
  std::string line;

  long pos_max = 0;
	long i = 0, i_max = 0;
  while (std::getline(infile, line)) {
		i++;
    if(line != "") {
      long pos = atol(line.substr(0, line.find(" ")).c_str());
      if(pos > pos_max) {
				pos_max = pos;
				i_max = i;
			}
    }
  }

  infile.clear();
  infile.seekg(0, std::ios::beg);
  membership.resize(pos_max+1);
  std::fill(membership.begin(), membership.end(), -1);

  while (std::getline(infile, line)) {
    if(line != "") {
      std::replace(line.begin(), line.end(), '\t', ' ');
      long pos = atol(line.substr(0, line.find(" ")).c_str());
      long community = strtol(line.substr(line.find(" ")+1, line.size()).c_str(), NULL, 10);
      //std::cout << community << std::endl;
      membership[pos] = community;
    }
  }
  for(long i =0; i < membership.size(); i++)
    if(membership[i] == -1)
      std::cout << "Warning : missing id " << i << " when reading file " << filename << std::endl;
}

void read_membership(std::string filename, std::map<long, std::list<long> >& membership) {
  std::ifstream infile;
  infile.open(filename.c_str(), std::ifstream::in);
  std::string line;
  for(long i = 0; i<membership.size(); i++) {
    std::list<long> l;
    //    membership[i] = *(new std::list<long>);
    membership[i] = l;
  }

  while(std::getline(infile, line)) {
    if(line != "") {
			std::replace(line.begin(), line.end(), '\t', ' ');
      std::stringstream ss(line);
      std::string item;
      std::getline(ss, item, ' ');
      long pos = atol(item.c_str());

      while(std::getline(ss, item, ' ')) {
				if(!item.empty()) {
					if(membership.find(pos) == membership.end()) {
						std::list<long> l;
						membership[pos] = l;
					}
					membership[pos].push_back(atol(item.c_str()));
				}
      }
    }
  }
}

void read_membership(std::string filename, std::vector<std::list<long> >& membership) {
  std::ifstream infile;
  infile.open(filename.c_str(), std::ifstream::in);
  std::string line;

  long pos_max = 0;
  while(std::getline(infile, line)) {
    if(line != "") {
      long pos = atol(line.substr(0, line.find(" ")).c_str());
      if(pos > pos_max)
				pos_max = pos;
    }
  }

  infile.clear();
  infile.seekg(0, std::ios::beg);
  membership.resize(pos_max+1);
  for(long i = 0; i<membership.size(); i++) {
    std::list<long> l;
    //    membership[i] = *(new std::list<long>);
    membership[i] = l;
  }

  while(std::getline(infile, line)) {
    if(line != "") {
      std::replace(line.begin(), line.end(), '\t', ' ');
      std::stringstream ss(line);
      std::string item;
      std::getline(ss, item, ' ');
      long pos = atol(item.c_str());

      while(std::getline(ss, item, ' ')) {
				if(!item.empty())
					membership[pos].push_back(atol(item.c_str()));
      }
    }
  }
}

/* print all the files and directories within directory that has the given prefix */
int list_files(std::string dirpath, std::string prefix, std::list<std::string>& output) {
  DIR *dir;
  
  if(dirpath[dirpath.size()-1] != '/') //The dir string always ends with a /
    dirpath += '/';

  struct dirent *ent;
  if ((dir = opendir (dirpath.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string filename = ent->d_name;
      if(filename.size() > prefix.size()) {
				std::pair<std::string::iterator, std::string::iterator> res = std::mismatch(prefix.begin(), prefix.end(), filename.begin());
				if(res.first == prefix.end()) { //Check if the filename features this prefix
					output.push_back(dirpath+filename);
				}
      }
    }
    closedir (dir);
    return 0;
  } 

  /* could not open directory */
  return 1;
}
