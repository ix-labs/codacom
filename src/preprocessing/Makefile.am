bin_PROGRAMS = simplify
if BOOST
	bin_PROGRAMS += ldfs_reordering
	ldfs_reordering_CPPFLAGS = -std=c++11 -O2 -lgraph
	ldfs_reordering_SOURCES = lexdfs/Graph.cpp lexdfs/Graph.hpp lexdfs/LexDFS.cpp lexdfs/LexDFS.hpp ldfs_reordering.cpp
endif
simplify_CPPFLAGS = $(IGRAPH_CFLAGS) -I$(top_srcdir)/src/libmisc
simplify_LDADD = $(IGRAPH_LIBS) $(top_builddir)/src/libmisc/libmisc.la
simplify_SOURCES = simplify.cpp
