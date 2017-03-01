<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <title>COmmunity Detection Algorithm COMparator (CoDACom)</title>

    <!-- Bootstrap -->
    <link href="bootstrap/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="main.css" rel="stylesheet">	

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
				 <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
				 <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
				 <![endif]-->
  </head>
  <body>

		<?php $file='index.php'; include_once('header.php');?>
    
		<div class="container">
			<div class="row">
				<div class="col-xs-hidden col-sm-3">
					<nav id="leftCol">
						<ul class="nav nav-pills nav-stacked" id="sidebar">
						</ul>
					</nav>
				</div>
				<div class="col-sm-9"> 
					<div class="jumbotron">
						<h2 id="overview">A community detection tool</h2>	
						CoDACom (Community Detection Algorithm Comparator) is a software which purpose is to simplify the research in community detection.
						It is designed to :
						<ul>
							<li>Run multiple community detection algorithms on graphs</li>
							<li>Output a set of statistics on these graphs and on the runs, from degree distribution to the quality of the different communities</li>
							<li>Take into account ground-truth by comparing it with the results of community detection</li>
							<li>Include home-made implementations of (potentially new) community detection algorithms with no code re-writing</li>
							<li>Include user-written quality functions</li>
							<li>Include user-written extrinsic comparison functions</li>
						</ul>
		  		</div>
					<h2 id="efficient">Efficient community detection</h2>
					The community detection implementations featured in CoDACom are, for the most part, written in C/C++.
					For the fastest (loglinear), it is entirely possible to analyse graphs with millions of edges on a standard desktop in less than an hour.
					Using a server grants the advantage of running all the couples (methods, graph) in parallel, which may significantly speed up the process.
					
					<h2 id="ground-truth">Ground-truth analysis</h2>
					If your graph features ground-truth communities, CoDACom compares them with the results of the community detection algorithms.
					This is useful to justify the superiority of a community detection algorithm in a context.
					On top of that, <a href="https://en.wikipedia.org/wiki/Spearman's_rank_correlation_coefficient">Spearman's coefficient</a> is computed, comparing the ranking of the algorithms to the ranking of the quality functions.
					This method is applied to justify the superiority of a quality function in a context.
					
					<h2 id="plug-play">Plug and play</h2>
					You have made your own algorithm, or an efficient implementation of an existing one, and want to compare this implementation to standard algorithms?
					You can just put the implementation in the correct folder, create a normalised shell script to run it with normalised input and output, et voilà.
					If you are familiar with autotools, you may even include your build with the main build.			
					
					<h2 id="extension">Extension</h2>
					Your own quality function can easily be implemented in CoDACom and applied in the system.			
					The same holds for a comparison method.			
					
					<h2 id="visu">Visualisation</h2>
					CoDACom can output a graphml per original graph, which can be viewed by most graph visualisation tools.
					For each applied community detection algorithm, a vertex and edge label is created representing the clustering.
					
					<h2 id="modularity">Modularity</h2>
					If only a module of CoDACom interests you and the rest is just taking up computation time, anything else can be deactivated.			
				</div>
			</div>
		</div>

    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="jquery.min.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
    <script src="bootstrap/js/bootstrap.min.js"></script>
    <!-- JavaScript jQuery code from Bootply.com editor  -->  
		<script src='main.js'></script>
  </body>
</html>
