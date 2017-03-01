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
#include "Parameters.hpp"
#include<cstring>
#include<iostream>
#include<cstdlib>
#include<boost/algorithm/string/case_conv.hpp>
#include<fstream>
//Base initialisation of the parameters
std::string Parameters::input_graph_file = "ext.graphml";
std::string Parameters::output_graph_file = "ext_out.graphml";
CommunityDetectionMethod Parameters::method = LexDFS;
LexDFSAnalyser Parameters::analyser = EdgeWeight;
long Parameters::nb_iters = 10;
std::string Parameters::dend_file = "";
bool Parameters::experiments[NB_RANGE][NB_QUALITY_FUNCTIONS] = 
{
  {false, false, false, false, false},
  {false, false, false, false, false}
};

std::string Parameters::experiments_string_range[NB_RANGE] = {"local", "global"};
std::string Parameters::experiments_string_quality[NB_QUALITY_FUNCTIONS] = {"conductance", "modularity", "compactness", "clust_coef", "size"};
std::ostream* Parameters::output_exp = &(std::cout);
QualityFunction Parameters::qf_to_max = Modularity;

void parameter_not_compatible(char* c) {
  std::cerr << "Parameter not compatible : " << c << std::endl;
  std::exit(1);
}

void unknown_option(char* c) {
  std::cerr << "Unknown option : " << c << std::endl;
  std::exit(1);
}

bool s_equ(char const* c1, char const* c2) {
  std::string s1(c1), s2(c2);
  return boost::to_upper_copy(s1).compare(boost::to_upper_copy(s2)) == 0;
}

/*parameters : 
-m lexdfs|lexdfs+ edgeweight|commonintervals
-input input
-output output
-nb_iters nb
-exp global|local compactness|modularity|conductance|clust_coef
-out_exp output_experiments_file
-max compactness|modularity|conductance
-dend filename*/
void Parameters::analyse_args(int argc, char* argv[]) {
  int i = 1;
  int nb_parameters = 1;
  while(i < argc-1) {
    if(s_equ(argv[i], "-m")) {
      if(i<argc-2 && argv[i+2][0] != '-') {
	if(s_equ(argv[i+2], "EDGEWEIGHT")) 
	  analyser = EdgeWeight;
	else if(s_equ(argv[i+2], "COMMONINTERVALS"))
	  analyser = CommonIntervals;
	else
	  parameter_not_compatible(argv[i+2]);
	nb_parameters = 2;
      }
      if(s_equ(argv[i+1], "LEXDFS"))
	method = LexDFS;
      else if(s_equ(argv[i+1], "LEXDFS+"))
	method = LexDFSPlus;
      else
	parameter_not_compatible(argv[i+1]);
    } else if(s_equ(argv[i], "-input")) {
      input_graph_file = argv[i+1];
    } else if(s_equ(argv[i], "-max")) {
      bool found = false;
      int k = 0;
      while(!found && k<NB_QUALITY_FUNCTIONS) {
	if(s_equ(argv[i+1], experiments_string_quality[k].c_str())) {
	  found = true;
	  qf_to_max = (QualityFunction)k;
	} else
	  k++;
      }
    } else if(s_equ(argv[i], "-output")) {
      output_graph_file = argv[i+1];
    } else if(s_equ(argv[i], "-exp")) {
      bool found = false;
      int k = 0, j = 0;
      while(!found && k<NB_RANGE) {
	if(s_equ(argv[i+1], experiments_string_range[k].c_str())) {
	  while(!found && j<NB_QUALITY_FUNCTIONS) {
	    if(s_equ(argv[i+2], experiments_string_quality[j].c_str())) {
	      experiments[k][j] = true;
	      found = true;
	    }
	    j++;
	  }
	  if(!found)
	    parameter_not_compatible(argv[i+2]);
	}
	k++;
      }
      if(!found)
	parameter_not_compatible(argv[i+1]);

      nb_parameters = 2;
    } else if(s_equ(argv[i], "-out_exp")) {
      std::ofstream* f = new std::ofstream();
      f->open(argv[i+1]);
      output_exp = f;
    } else if(s_equ(argv[i], "-nb_iters")) {
      nb_iters = atol(argv[i+1]);
    } else if(s_equ(argv[i], "-dend")) {
      dend_file = argv[i+1];
    } else {
      unknown_option(argv[i]);
    }
    i += nb_parameters+1;
    nb_parameters = 1;
  }
}

std::string Parameters::getInputGraphFile() {
  return input_graph_file;
}
std::string Parameters::getOutputGraphFile() {
  return output_graph_file;
}

std::string Parameters::getDendFile() {
  return dend_file;
}

QualityFunction Parameters::getQfToMax() {
  return qf_to_max;
}

CommunityDetectionMethod Parameters::getMethod() {
  return method;
}

std::ostream& Parameters::getExpOutStream() {
  return *output_exp;
}

bool Parameters::isExpToDo(QualityFunction q, Range r) {
  return experiments[r][q];
}

std::string Parameters::name(QualityFunction q) {
  return experiments_string_quality[q];
}

std::string Parameters::name(Range r) {
  return experiments_string_range[r];
}

long Parameters::getNbIters() {
  return nb_iters;
}

LexDFSAnalyser Parameters::getAnalyser() {
  return analyser;
}

Parameters::~Parameters() {
  if(output_exp != &std::cout) {
    ((std::ofstream*)output_exp)->close();
    delete output_exp;
  }
}
