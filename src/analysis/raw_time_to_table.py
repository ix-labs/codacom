# Copyright GREYC - UMR 6072 ; Université de Caen Normandie
# Esplanade de la paix
# CS 14032
# 14032 Caen CEDEX 5
# contributeur :
# Jean Creusefond, 2015
# 
# jean.creusefond@unicaen.fr
# 
# Ce logiciel est un programme informatique servant à la comparaison et à l'analyse d'outils de détection de communauté.
# 
# Ce logiciel est régi par la licence CeCILL-B soumise au droit français et respectant les principes de diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou redistribuer ce programme sous les conditions de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie, de modification et de redistribution accordés par cette licence, il n'est offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons, seule une responsabilité restreinte pèse sur l'auteur du programme, le titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard  l'attention de l'utilisateur est attirée sur les risques
# associés au chargement,  à l'utilisation,  à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels avertis possédant  des  connaissances  informatiques approfondies.  Les utilisateurs sont donc invités à charger  et  tester  l'adéquation  du logiciel à leurs besoins dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL-B, et que vous en avez accepté les termes.
# 
# ==========================================================================
# 
# Copyright GREYC - UMR 6072 ; Université de Caen Normandie
# Esplanade de la paix
# CS 14032
# 14032 Caen CEDEX 5
# contributor(s) :
# Jean Creusefond, 2015
# 
# jean.creusefond@unicaen.fr
# 
# This software is a computer program whose purpose is to compare and analyse community detection tools.
# 
# This software is governed by the CeCILL-B license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-B
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".
# 
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
# 
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
# 
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-B license and that you accept its terms. 
import sys

def print_help(s):
    print(s, "\n")
    print("Usage : python3 raw_time_to_table.py infile outfile")
    print("Help : converts a raw time file (ALG GRAPH TIME) to a table file (rows : alg, column : graph, value : time)\n")
    sys.exit(1)

infile=""
outfile=""
if len(sys.argv) > 2 :
    infile = sys.argv[1]
    outfile = sys.argv[2]
else:
    print_help("Not enough arguments")

rows = {}
graphs = {}
f_in = open(infile, 'r')
for line in f_in:
    line = line.strip()
    if line != "":
        t_line = line.split()
        alg = t_line[0]
        graph = t_line[1]
        time = t_line[2]
        
        if alg not in rows.keys():
            rows[alg] = {}
        rows[alg][graph] = time
        graphs[graph] = True
f_in.close()

l_graphs = [k for k in graphs.keys()]
l_graphs.sort()

f_out = open(outfile, 'w')
f_out.write("method\\graph")
for g in l_graphs:
    f_out.write(" " + g)

for a in rows:
    f_out.write("\n" + a)
    for g in l_graphs:
        if g in rows[a].keys():
            f_out.write(" " + rows[a][g])
        else:
            f_out.write(" -")
f_out.write("\n")