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
#include <igraph/igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <libgen.h>
#include <string.h>

#include "algorithms.h"

void print_vector(igraph_vector_t *v, FILE *f) {
  long int i;
  for (i=0; i<igraph_vector_size(v); i++) {
    fprintf(f, "%li %f\n", i, VECTOR(*v)[i]);
  }
  fprintf(f, "\n");
}

void print_vector_in_file(igraph_vector_t *v, char *outdir, char *infile, const char *mark) {
  std::string path(outdir);
  if(path[path.size()-1] != '/')
    path += '/';

  std::string s_file(basename(infile));
  s_file.erase(s_file.rfind('.'), std::string::npos); //removes the extention
  path += s_file + '_' + std::string(mark);

  FILE* outfile = fopen(path.c_str(), "w+");
  print_vector(v, outfile);
  fclose(outfile);
}

void usage(const char *more) {
  std::cerr << more;
  std::cerr << "usage: -i input_file -o outdir [-a algorithm]*" << std::endl << std::endl;
  std::cerr << "Applies the algorithms on the input graph, outputs the membership" << std::endl;
  std::cerr << "-i input_file\tInput graph file (list of edges)" << std::endl;
  std::cerr << "-o output_dir\tDirectory where the output will be stored" << std::endl;
  std::cerr << "-a algorithm\tAlgorithm used to produce the membership files. Choices are : " << std::endl;
  std::cerr << "\tClauset : Greedy modularity optimisation" << std::endl;
  exit(1);
}

char *infile   = NULL;
char *outdir  = NULL;

#define NB_ALGS 7
bool algs_apply[NB_ALGS] = 
  {false, false, false, false, false, false, false};
void (*algs_call[NB_ALGS])(igraph_t* graph, igraph_vector_t* membership) =
{clauset, leading_eigenvector, spinglass, walktrap, betweenness, label_prop, infomap};


void parse_args(int argc, char **argv, std::string* algs_name) {
  for (int i = 1; i < argc; i++) {
    bool found = false;
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'i':
	if (i==argc-1)
	  usage("Infile missing\n");
	infile = argv[i+1];
	i++;
	break;
      case 'o':
	if (i==argc-1)
	  usage("Outdir missing\n");
        outdir = argv[i+1];
	i++;
	break;
      case 'a' :
        if (i==argc-1)
	  usage("Algorithm missing\n");
	for(int j = 0; j < NB_ALGS;j++) {
	  if(strcmp(argv[i+1], algs_name[j].c_str()) == 0) {
	    algs_apply[j] = true;
	    found = true;
	    break;
	  }
	}
	if(!found)
	  usage("Algorithm not recognized\n");
	i++;
	break;
      default:
	usage("Unknown option\n");
      }
    } else {
      usage("More than one filename\n");
    }
  }
  if (infile==NULL || outdir==NULL)
    usage("In or outfile missing\n");
}

int main(int argc, char **argv) {
  std::string * algs_name = new std::string[NB_ALGS]; 
  algs_name[0] = "clauset";
  algs_name[1] = "leading_eigenvector";
  algs_name[2] = "spinglass";
  algs_name[3] = "walktrap";
  algs_name[4] = "betweenness";
  algs_name[5] = "label_prop";
  algs_name[6] = "infomap";
  
  parse_args(argc, argv, algs_name);
  igraph_t graph;

  FILE* in = fopen(infile, "r");
  igraph_read_graph_edgelist(&graph, in, 0, false);
  fclose(in);
  
  for(int i =0; i < NB_ALGS; i++) {
    if(algs_apply[i]) {
      igraph_vector_t membership;
      igraph_vector_init(&membership, 0);
      algs_call[i](&graph, &membership);
      print_vector_in_file(&membership, outdir, infile, algs_name[i].c_str());
    }
  }

  return 0;
}

