AM_CPPFLAGS = -I$(top_srcdir)/src/libmisc

noinst_LTLIBRARIES = libcomparison.la
libcomparison_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/comparison $(IGRAPH_CFLAGS)
libcomparison_la_LIBADD = $(top_builddir)/src/libmisc/libmisc.la
libcomparison_la_SOURCES = comparison/AbstractCM.cpp
include targets_libcomparison
libcomparison_la_SOURCES += CMProvider.cpp

noinst_HEADERS = comparison/AbstractCM.hpp
include headers_libcomparison
noinst_HEADERS += CMProvider.hpp
