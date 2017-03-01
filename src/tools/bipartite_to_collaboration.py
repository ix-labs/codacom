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
    print("Usage : python3 bipartite_to_collaboration.py in_file out_g out_gt [p]")
    print("Help : converts a bipartite graph to a collaboration graph (an edge exists if two persons were on the same project, with probability p) and a ground_truth file\n")
    sys.exit(1)

s_in = ""
s_out_g = ""
s_out_gt = ""
p = 1
if len(sys.argv) > 3 :
    s_in = sys.argv[1]
    s_out = sys.argv[2]
    s_out_gt = sys.argv[3]
    if len(sys.argv) > 4:
        p = float(sys.argv[4])
else:
    print_help("Not enough arguments")

if p < 0 or p > 1:
    print_help("As a probability, p must be between 0 and 1")

f_in = open(s_in, 'r')
membership = {}
collaboration = {}
for line in f_in:
    line = line.strip("\n")
    nodes = line.replace('\t', ' ').replace(';', ' ').split(" ")
    v = nodes[0]
    u = nodes[1]
    if v not in membership.keys():
        membership[v] = [u]
    else:
        membership[v] += [u]
    if u not in collaboration.keys():
        collaboration[u] = [v]
    else:
        collaboration[u] += [v]
f_in.close()

#Outputting ground truth
f_out_gt = open(s_out_gt, 'w')
s = 0
for v in membership:
    f_out_gt.write(v)
    for u in membership[v]:
        s += len(collaboration[u]) - 1
        f_out_gt.write(" " + u)
    f_out_gt.write('\n')
f_out_gt.close()

print("Avg number of edges", s/len(membership), "Number of users", len(membership))
print("Total number of collaborations", len(collaboration))

#Creating edge list
l = {}
for u in collaboration:
    c = collaboration[u]
    for i in range(len(c)):
        for j in range(i+1, len(c)):
            u = int(c[i])
            v = int(c[j])
            if v < u:
                temp = u
                u = v
                v = temp
            s = str(u) + " " + str(v)
            l[s] = True

#Outputting edge list
f_out = open(s_out, 'w')
for e in l:
    f_out.write(e + '\n')
f_out.close()
                
        
