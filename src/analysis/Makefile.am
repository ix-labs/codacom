AM_CPPFLAGS = -I$(top_srcdir)/src/libmisc -I$(top_srcdir)/src/libquality -I$(top_srcdir)/src/libcomparison

bin_PROGRAMS = algs_quality algs_correlation alg_full_report

algs_quality_CPPFLAGS = $(AM_CPPFLAGS)
algs_quality_LDADD = $(IGRAPH_LIBS) $(top_builddir)/src/libmisc/libmisc.la $(top_builddir)/src/libquality/libquality.la $(top_builddir)/src/libcomparison/libcomparison.la
algs_quality_SOURCES = algs_quality.cpp

algs_correlation_CPPFLAGS = $(AM_CPPFLAGS)
algs_correlation_LDADD = $(IGRAPH_LIBS) $(top_builddir)/src/libcomparison/libcomparison.la
algs_correlation_SOURCES = algs_correlation.cpp

alg_full_report_CPPFLAGS = $(AM_CPPFLAGS)
alg_full_report_LDADD = $(IGRAPH_LIBS) $(top_builddir)/src/libmisc/libmisc.la $(top_builddir)/src/libquality/libquality.la
alg_full_report_SOURCES = alg_full_report.cpp

#dist_SCRIPTS = 
EXTRA_DIST = mean_results.py quality_to_rank.py raw_time_to_table.py collect_spearman.py
