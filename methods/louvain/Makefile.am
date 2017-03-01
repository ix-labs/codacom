if LOUVAIN
bin_PROGRAMS=community convert hierarchy
AM_CPPFLAGS=-ansi -O5 -Wall
AM_LDFLAGS=-ansi -lm -Wall

community_CPPFLAGS=$(AM_CPPFLAGS)
community_LDFLAGS=$(AM_LDFLAGS)
community_SOURCES=graph_binary.h community.h graph_binary.cpp community.cpp main_community.cpp

convert_CPPFLAGS=$(AM_CPPFLAGS)
convert_LDFLAGS=$(AM_LDFLAGS)
convert_SOURCES=graph.h graph.cpp main_convert.cpp

hierarchy_CPPFLAGS=$(AM_CPPFLAGS)
hierarchy_LDFLAGS=$(AM_LDFLAGS)
hierarchy_SOURCES=main_hierarchy.cpp
endif

EXTRA_DIST = execute.sh.inst

dist-hook:
	rm $(distdir)/*.h $(distdir)/*.cpp
