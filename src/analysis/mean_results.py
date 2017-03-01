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
import os
import sys
import math
import statistics as stats

def print_help(s):
    print(s, "\n")
    print("Usage : python3 mean_results.py indir prefix outfile")
    print("Help : computes the mean matrix of the files in indir that have the set prefix, outputs in outfile\n")
    sys.exit(1)

# def mean(data, i, j):
#     res = 0
#     k = 0
#     for mat in data:
#         k+=1
#         res=res*(k-1)/k + mat[i][j]/k
#     return res

# def stddev(data, i, j):
#     res = 0
#     k = 0
#     avg = mean(data, i, j)
#     for mat in data:
#         k+=1
#         res=res*(k-1)/k + (mat[i][j] - avg)*(mat[i][j] - avg)/k
#     return math.sqrt(res)

indir=""
prefix=""
outfile=""
if len(sys.argv) > 3 :
    indir = sys.argv[1]
    prefix = sys.argv[2]
    outfile = sys.argv[3]
else:
    print_help("Not enough arguments")

if not indir.endswith('/'):
    indir += "/"

f_out = open(outfile, 'w')
results = {}

for file in os.listdir(indir):
    if file.startswith(prefix):
        f_in = open(indir+file, 'r')
        top = f_in.readline().strip().split(" ")
        for i in range(1, len(top)):
            if not top[i] in results.keys():
                results[top[i]] = {}

        for line in f_in:
            line = line.strip()
            if line != "":
                t_line = line.split()
                ltitle = t_line[0]
                for i in range(1, len(t_line)):
                    m = results[top[i]]
                    if ltitle in m.keys():
                        m[ltitle] += [float(t_line[i])]
                    else:
                        m[ltitle] = [float(t_line[i])]
        f_in.close()

f_out.write(prefix)
tops = []
side = {}
for k in results.keys():
    tops+=[k]
    f_out.write(" " + k)                
    for s in results[k].keys():
        side[s] = 0
f_out.write("\n")

for k in side.keys():
    f_out.write(k)
    for t in tops:
        if k in results[t].keys():
            f_out.write(" " + str("%.2f" % stats.mean(results[t][k])) + "+-" + str("%.2f" % stats.stdev(results[t][k])))
        else:
            f_out.write(" -")
    f_out.write("\n")
