EXTRA_DIST = template.sh execute.sh timeout.sh betweenness/execute.sh clauset/execute.sh infomap/execute.sh label_prop/execute.sh leading_eigenvector/execute.sh mcl/execute.sh spinglass/execute.sh walktrap/execute.sh conclude/execute.sh.inst scd/execute.sh.inst
#EXTRA_DIST = template.sh execute.sh timeout.sh conclude mcl
# ##ADDING GRACLUS
# EXTRA_DIST += graclus/execute.sh graclus/Makefile.in graclus/matlab graclus/metisLib graclus/multilevelLib graclus/programs
#SUBDIRS = igraph louvain graclus
SUBDIRS = igraph lexdfs k-core louvain

EXTRA_DIST += scrap_web.sh

dist-hook:
	rm -rf `find $(distdir)/* -type d -name .svn`
