bin_PROGRAMS = list_qf list_cm

list_qf_SOURCES = list_qualities.cpp
list_qf_LDADD = $(top_builddir)/src/libquality/libquality.la
list_qf_CPPFLAGS = -I$(top_srcdir)/src/libquality

list_cm_SOURCES = list_comparison_methods.cpp
list_cm_LDADD = $(top_builddir)/src/libcomparison/libcomparison.la
list_cm_CPPFLAGS = -I$(top_srcdir)/src/libcomparison

EXTRA_DIST = bipartite_to_collaboration.py change_var.sh test_compact.py membership_to_multi.py unicolumn_to_bicolumn.py communities_to_membership.py reorder_edges_with_raw.py edgelist_to_adjacency.py
