if BOOST
    noinst_LIBRARIES = libsearch.a
    noinst_HEADERS = search/Graph.hpp search/QualityMaximisation.hpp search/LexDFS.hpp search/CommunityDetection.hpp search/Parameters.hpp search/CommonIntervalDetection.hpp search/EdgeWeightDetection.hpp search/OverlappingEdgeColors.hpp
    libsearch_a_CPPFLAGS = -std=c++11 -O2 -lgraph
    libsearch_a_SOURCES = search/Graph.cpp search/QualityMaximisation.cpp search/LexDFS.cpp search/CommunityDetection.cpp search/Parameters.cpp search/CommonIntervalDetection.cpp search/EdgeWeightDetection.cpp search/OverlappingEdgeColors.cpp

    bin_PROGRAMS = lexdfs import_dendogram
    lexdfs_CPPFLAGS = -std=c++11 -O2 -lgraph
    lexdfs_LDADD = libsearch.a
    lexdfs_SOURCES = main.cpp

    import_dendogram_CPPFLAGS = -std=c++11 -O2 -lgraph
    import_dendogram_LDADD = libsearch.a
    import_dendogram_SOURCES = import_dendogram.cpp

endif

EXTRA_DIST = execute.sh
