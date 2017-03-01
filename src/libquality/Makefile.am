noinst_LTLIBRARIES = libquality.la
libquality_la_CPPFLAGS = -I$(srcdir)/quality $(IGRAPH_CFLAGS)
libquality_la_SOURCES = quality/AbstractQF.cpp quality/AbstractAgglomerativeQF.cpp quality/AbstractSVQF.cpp quality/AbstractSCQF.cpp quality/MeanQF.cpp quality/SumQF.cpp
include targets_libquality
libquality_la_SOURCES += QFProvider.cpp

noinst_HEADERS = quality/AbstractQF.hpp quality/AbstractAgglomerativeQF.hpp quality/AbstractSVQF.hpp quality/AbstractSCQF.hpp quality/MeanQF.hpp quality/SumQF.hpp
include headers_libquality
noinst_HEADERS += QFProvider.hpp

EXTRA_DIST = quality/TemplateSCQF.cpp.templ quality/TemplateSCQF.hpp.templ
