##igraph 0.6 write_graph_graphml function has 2 args whilst 0.7 has 3 args
AC_DEFUN([AX_FUNC_WRITE_GRAPHML],
	[AC_CHECK_FUNCS([igraph_write_graph_graphml])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_WRITE_GRAPHML_TWO_ARGS
	])

# _AX_FUNC_MKDIR_ONE_ARG(IF-ONE-ARG, IF-TWO-ARGS)
# -----------------------------------------------
# Execute IF-TWO-ARGS if mkdir() accepts two
# arguments; execute IF-ONE-ARG otherwise.
AC_DEFUN([_AX_FUNC_WRITE_GRAPHML_TWO_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	#include <stdio.h>
	igraph_t graph;
	FILE* out;
	]], [[igraph_write_graph_graphml(&graph, out, 0);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_WRITE_GRAPHML_TWO_ARGS],
	[AC_CACHE_CHECK([whether writing a graphml with igraph takes two args],
	[ax_cv_write_graphml_two_args],
	[_AX_FUNC_WRITE_GRAPHML_TWO_ARGS([ax_cv_write_graphml_two_args=yes],
	[ax_cv_write_graphml_two_args=no])])
	if test x"$ax_cv_write_graphml_two_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([WRITE_GRAPHML_TWO_ARGS], [1],
	      [Define if writing a graphml with igraph takes two arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_WRITE_GRAPHML_TWO_ARGS


##igraph leading_eigenvector function has 12 args in 0.6 and 13 in 0.7
AC_DEFUN([AX_FUNC_LEAD_EIG],
	[AC_CHECK_FUNCS([igraph_community_leading_eigenvector])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_LEAD_EIG_ELEVEN_ARGS
	])

# _AX_FUNC_LEAD_EIG_ELEVEN_ARGS(IF-ELEVEN-ARGS, IF-TWELVE-ARGS)
# -----------------------------------------------
# Execute IF-ELEVEN-ARGS if igraph_community_leading_eigenvector() accepts eleven
# arguments; execute IF-TWELVE-ARGS otherwise.
AC_DEFUN([_AX_FUNC_LEAD_EIG_ELEVEN_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	igraph_t* graph;
	igraph_vector_t* membership;
	igraph_arpack_options_t options;
	igraph_matrix_t merges;
	]], [[igraph_community_leading_eigenvector(graph, /*weights=*/ 0, &merges,
	    membership, igraph_vcount(graph),
	    &options, /*modularity=*/ 0,
	    /*start=*/ 0, /*eigenvalues=*/ 0,
	    /*eigenvectors=*/ 0, /*history=*/ 0,
	    /*callback=*/ 0,
	    /*callback_extra=*/ 0);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_LEAD_EIG_ELEVEN_ARGS],
	[AC_CACHE_CHECK([whether the igraph leading eigenvector method has eleven args],
	[ax_cv_lead_eig_eleven_args],
	[_AX_FUNC_LEAD_EIG_ELEVEN_ARGS([ax_cv_lead_eig_eleven_args=yes],
	[ax_cv_lead_eig_eleven_args=no])])
	if test x"$ax_cv_lead_eig_eleven_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([LEAD_EIG_ELEVEN_ARGS], [1],
	      [Define if the igraph leading eigenvector takes eleven arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_LEAD_EIG_ELEVEN_ARGS

##igraph get_eid function has 5 args in 0.5 and 6 afterwards
AC_DEFUN([AX_FUNC_GET_EID],
	[AC_CHECK_FUNCS([igraph_get_eid])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_GET_EID_FIVE_ARGS
	])

# _AX_FUNC_GET_EID_FIVE_ARGS(IF-FIVE-ARGS, IF-SIX-ARGS)
# -----------------------------------------------
# Execute IF-FIVE-ARGS if igraph_get_eid() accepts five
# arguments; execute IF-SIX-ARGS otherwise.
AC_DEFUN([_AX_FUNC_GET_EID_FIVE_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	igraph_t* graph;
	igraph_integer_t from = 0, to = 0, eid;
	]], [[igraph_get_eid(graph, &eid, from, to, 0, 0);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_GET_EID_FIVE_ARGS],
	[AC_CACHE_CHECK([whether the igraph get eid method has five args],
	[ax_cv_get_eid_five_args],
	[_AX_FUNC_GET_EID_FIVE_ARGS([ax_cv_get_eid_five_args=yes],
	[ax_cv_get_eid_five_args=no])])
	if test x"$ax_cv_get_eid_five_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([GET_EID_FIVE_ARGS], [1],
	      [Define if the igraph get eid takes five arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_GET_EID_FIVE_ARGS

##igraph simplify function has 3 args in 0.5 and 4 afterwards
AC_DEFUN([AX_FUNC_SIMPLIFY],
	[AC_CHECK_FUNCS([igraph_simplify])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_SIMPLIFY_THREE_ARGS
	])

# _AX_FUNC_SIMPLIFY_THREE_ARGS(IF-THREE-ARGS, IF-FOUR-ARGS)
# -----------------------------------------------
# Execute IF-THREE-ARGS if igraph_simplify() accepts three
# arguments; execute IF-FOUR-ARGS otherwise.
AC_DEFUN([_AX_FUNC_SIMPLIFY_THREE_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	igraph_t* graph;
	]], [[igraph_simplify(graph, 0, 0, NULL);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_SIMPLIFY_THREE_ARGS],
	[AC_CACHE_CHECK([whether the igraph simplify method has three args],
	[ax_cv_simplify_three_args],
	[_AX_FUNC_SIMPLIFY_THREE_ARGS([ax_cv_simplify_three_args=yes],
	[ax_cv_simplify_three_args=no])])
	if test x"$ax_cv_simplify_three_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([SIMPLIFY_THREE_ARGS], [1],
	      [Define if the igraph get eid takes three arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_SIMPLIFY_THREE_ARGS

##igraph igraph_bfs function has 6 args in 0.5 and many more afterwards
AC_DEFUN([AX_FUNC_IGRAPH_BFS],
	[AC_CHECK_FUNCS([igraph_bfs])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_IGRAPH_BFS_SIX_ARGS
	])

# _AX_FUNC_IGRAPH_BFS_SIX_ARGS(IF-SIX-ARGS, IF-MANY-ARGS)
# -----------------------------------------------
# Execute IF-SIX-ARGS if igraph_bfs() accepts six
# arguments; execute IF-FOUR-ARGS otherwise.
AC_DEFUN([_AX_FUNC_IGRAPH_BFS_SIX_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	igraph_t* graph;
	]], [[igraph_bfs(&graph, 0, NULL, IGRAPH_ALL, 0, NULL,
	       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_IGRAPH_BFS_SIX_ARGS],
	[AC_CACHE_CHECK([whether the igraph igraph_bfs method has six args],
	[ax_cv_igraph_bfs_six_args],
	[_AX_FUNC_IGRAPH_BFS_SIX_ARGS([ax_cv_igraph_bfs_six_args=yes],
	[ax_cv_igraph_bfs_six_args=no])])
	if test x"$ax_cv_igraph_bfs_six_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([IGRAPH_BFS_SIX_ARGS], [1],
	      [Define if the igraph get eid takes six arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_IGRAPH_BFS_SIX_ARGS

##igraph igraph_community_fastgreedy function has 4 args in 0.5 and many more afterwards
AC_DEFUN([AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY],
	[AC_CHECK_FUNCS([igraph_community_fastgreedy])
	AC_CHECK_HEADERS([igraph/igraph.h])
	AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS
	])

# _AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS(IF-FOUR-ARGS, IF-MANY-ARGS)
# -----------------------------------------------
# Execute IF-FOUR-ARGS if igraph_community_fastgreedy() accepts four
# arguments; execute IF-FOUR-ARGS otherwise.
AC_DEFUN([_AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS],
	[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	#include <igraph/igraph.h>
	igraph_t* graph;
	]], [[igraph_community_fastgreedy(&graph, 0, NULL, IGRAPH_ALL, 0, NULL,
	       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);]])], [$2], [$1])])

AC_DEFUN([AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS],
	[AC_CACHE_CHECK([whether the igraph igraph_community_fastgreedy method has four args],
	[ax_cv_igraph_community_fastgreedy_four_args],
	[_AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS([ax_cv_igraph_community_fastgreedy_four_args=yes],
	[ax_cv_igraph_community_fastgreedy_four_args=no])])
	if test x"$ax_cv_igraph_community_fastgreedy_four_args" = xyes
	then
#  	   AC_LANG_PUSH([C++])
	   AC_DEFINE([IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS], [1],
	      [Define if the igraph get eid takes four arguments.])
#	   AC_LANG_POP([C++])
	fi]) # AX_FUNC_IGRAPH_COMMUNITY_FASTGREEDY_FOUR_ARGS
