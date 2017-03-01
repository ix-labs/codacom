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
    print("Usage : python3 collect_spearman.py outfile_prefix [rank_files]+")
    print("Help : Takes all the rank files. Detects if a spearman correlation has been done. \n")
    sys.exit(1)

out_prefix=""
if len(sys.argv) > 2 :
    out_prefix = sys.argv[1]
else:
    print_help("Not enough arguments")

if out_prefix[-1] != '_':
    out_prefix += '_'

#READ FILE
qualities = {}
for i in range(2, len(sys.argv)):
    s_file = sys.argv[i]
    f_in = open(s_file, 'r')
    if s_file.rfind('/') > -1:
        s_file = s_file[s_file.rfind('/')+1:]
    if s_file.startswith("rank_quality_"):
        s_file = s_file[len("rank_quality_"):]

    header = f_in.readline()
    quals = header.rstrip("\n").replace('\t', ' ').split(' ')[1:]
    
    for line in f_in:
        a = line.rstrip("\n").replace('\t', ' ').split(' ')
        if a[0].startswith("spearman_"):
            comparison = a[0][len("spearman_"):-len("_gt")]
            #print(comparison)
            if comparison not in qualities.keys():
                qualities[comparison] = {}
            for j in range(1, len(a)):
                q = quals[j-1]
                if q not in qualities[comparison].keys():
                    qualities[comparison][q] = {}
                qualities[comparison][q][s_file] = a[j]
    f_in.close()
        
for comparison in qualities:
    f_out = open(out_prefix + comparison, 'w')
    q_array = []
    files = {}
    for q in qualities[comparison]:
        q_array += [q]
        for f in qualities[comparison][q]:
            files[f] = True
    q_array.sort()
    
    f_array = []
    for f in files:
        f_array += [f]
    f_array.sort()

    f_out.write("file\quality")
    for q in q_array:
        f_out.write(" " + q)
    f_out.write('\n')
    
    for f in f_array:
        f_out.write(f)
        for q in q_array:
            if f in qualities[comparison][q].keys():
                f_out.write(" " + qualities[comparison][q][f])
            else:
                f_out.write(" -")
        f_out.write('\n')
    f_out.close()

#Correlation between graphs (do they give similar rankings to functions?)
#This only works if the number of quality functions is the same for each comparison function
table = {}
a_files = []
first = True
for comparison in qualities:
    table[comparison] = {}
    files = {}
    for q in qualities[comparison]:
        for f in qualities[comparison][q]:
            if f in files.keys():
                files[f] += [(q, float(qualities[comparison][q][f]))]
            else:
                files[f] = [(q, float(qualities[comparison][q][f]))]
                if first:
                    a_files += [f]
    a_files.sort()
    
    rank = {}
    for f in files:
        files[f].sort(key=lambda x:x[1], reverse=True)

        rank[f] = {}
        for i in range(len(files[f])):
            k = i
            v = files[f][k][1]
            while(k-1 >= 0 and v == files[f][k-1][1]):
                k-=1
            for l in range(k, i+1):
                q = files[f][l][0]
                rank[f][q] = (i+k)/2

    

    f_out = open(out_prefix + comparison + "_graphs", 'w')
    f_out.write("file\\file")
    for f in a_files:
        f_out.write(" " + f)
    f_out.write("\n")
    
    for i in range(len(a_files)):
        table[comparison][i] = {}
        f1 = a_files[i]
        f_out.write(f1)
        for j in range(0, i+1):
            f_out.write(" -")
            
        for j in range(i+1, len(a_files)):
            f2 = a_files[j]
            sum_di_2 = 0
            for q in rank[f1]:
                sum_di_2 += (rank[f1][q] - rank[f2][q])*(rank[f1][q] - rank[f2][q])
            n = len(rank[f1])
            s = 1 - 6*sum_di_2/(n*(n*n - 1))
            table[comparison][i][j] = s
            f_out.write(" " + str('%.3f' % s))
        f_out.write("\n")
    f_out.close()
    first = False

#Two by two comparison
comps = []
for comparison in qualities:
    comps += [comparison]

for i in range(len(comps)):
    c1 = comps[i]
    for j in range(i+1, len(comps)):
        c2 = comps[j]
        f_out = open(out_prefix + c1 + "_" + c2, 'w')
        f_out2 = open(out_prefix + c1 + "_" + c2 + "_min", 'w')
        f_out.write("file\\file")
        f_out2.write("file\\file")
        for f in a_files:
            f_out.write(" " + f)
            f_out2.write(" " + f)
        f_out.write("\n")
        f_out2.write("\n")
        for k in table[c1]:
            f_out.write(a_files[k]);
            f_out2.write(a_files[k]);
            for l in range(0, k+1):
                f_out.write(" -")
                f_out2.write(" -")
            for l in table[c1][k]:
                f_out.write(" " + str('%.3f' % abs(table[c1][k][l] - table[c2][k][l])))
                f_out2.write(" " + str('%.3f' % min(table[c1][k][l], table[c2][k][l])))
            f_out.write("\n")
            f_out2.write("\n")
        f_out.close()
        f_out2.close()
                

#print(qualities)
