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
    print("Usage : python3 quality_to_rank.py infile outfile")
    print("Help : converts quality file to rank.\n")
    sys.exit(1)

infile=""
outfile=""
if len(sys.argv) > 2 :
    infile = sys.argv[1]
    outfile = sys.argv[2]
else:
    print_help("Not enough arguments")

#READ FILE
a_qualities = []
f_in = open(infile, 'r')
header = f_in.readline()

gts = []
i = 1
a_header = header.rstrip("\n").split(" ")
qualities = {}
while a_header[i].endswith("_gt"):
    gts += [a_header[i]]
    a_qualities += [a_header[i]]
    qualities[a_header[i]] = []
    i += 1
while i < len(a_header):
    a_qualities += [a_header[i]]
    qualities[a_header[i]] = []
    i+=1

a_algs = []
k = 0
for line in f_in:
    values = line.rstrip("\n").split(" ")
    alg = values[0]
    values = values[1:]

    a_algs += [alg]
    i = 0
    for i in range(len(values)):
        qualities[a_qualities[i]] += [(alg,float(values[i]))]
        i+=1
    k+=1
f_in.close()

#RANKING
q_res = {}
for q in qualities:
    q2 = sorted(qualities[q], key=lambda q:q[1], reverse=True)
    q_res[q] = {}
    for i in range(len(q2)):
        k = i
        v = q2[i][1]
        while(k-1 >= 0 and v == q2[k-1][1]):
            k-=1
            v = q2[k][1]
        for l in range(k, i+1):
            a = q2[l][0]
            q_res[q][a] = (k+i)/2 + 1

# print(a_header)
# print(a_qualities)
# print(q_res)

#SPEARMAN
gt_spearman = {}
if len(a_algs) > 1:
    for gt in gts:
        gt_spearman[gt] = {}
        spearman=[]
        #We take the first column to be the reference quality
        n=len(a_algs)
        for q in q_res:
            sum_d_sq = 0
            for a in q_res[q]:
                r = q_res[gt][a]
                i = q_res[q][a]
                sum_d_sq += (i-r)*(i-r)
            tot = 1 - 6*sum_d_sq/(n*(n*n - 1))
            gt_spearman[gt][q] = tot

#OUTPUT
f_out = open(outfile, 'w')
f_out.write(header)
for i in range(len(a_algs)): #Loop over algs
    a = a_algs[i]
    f_out.write(a)
    for q in a_qualities: #Loop over qualities
        f_out.write(" " + str('%.2f' % q_res[q][a]))
    f_out.write("\n")
for gt in gt_spearman:
    f_out.write("spearman_"+gt)
    for q in a_qualities:
        f_out.write(" " + str('%.2f' % gt_spearman[gt][q]))
    f_out.write("\n")
f_out.close()
