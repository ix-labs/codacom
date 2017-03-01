AM_CPPFLAGS = -I$(top_srcdir)/src/libmisc $(IGRAPH_CFLAGS)
AM_LDADD = $(top_builddir)/src/libmisc/libmisc.la $(IGRAPH_LIBS)

bin_PROGRAMS = clustering_to_graphml clustering_to_quotient
clustering_to_graphml_CPPFLAGS = $(AM_CPPFLAGS)
clustering_to_graphml_LDADD = $(AM_LDADD)
clustering_to_graphml_SOURCES = clustering_to_graphml.cpp

clustering_to_quotient_CPPFLAGS = $(AM_CPPFLAGS)
clustering_to_quotient_LDADD = $(AM_LDADD)
clustering_to_quotient_SOURCES = clustering_to_quotient.cpp

#dist_SCRIPTS = 
EXTRA_DIST = data_to_tex.py
