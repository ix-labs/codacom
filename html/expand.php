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
  
		<?php $file='expand.php'; include_once('header.php');?>

		<div class="container">
			<div class="row">
				
				<div class="col-xs-hidden col-sm-3">
					<nav id="leftCol">
						<ul class="nav nav-pills nav-stacked" id="sidebar">
						</ul>
					</nav>
				</div><!--/left-->
				
	      <div class="col-sm-9">
					<h2 id="method">Add a method</h2>

					<p>
						Adding a method is simply about placing the right files at the right place.
						First, locate your METHOD_DIR folder, <span class="file">methods</span> by default.
						Add a folder for your method.
						In this folder, place whatever files are needed for your method to be run : scripts, binaries, etc.
						Then, add an <span class="file">execute.sh</span> file in this folder.
						This file will be detected by the program in order to reference your method (you may update the whitelist right away).
						During the execution of CoDACom, execute.sh will be run from the directory of your method.
						It is therefore the script that will input a graph, run your method, and output a clustering.
					</p>

					<p>
						It is advised to include <span class="file">template.sh</span>, located in the method folder, at the start of your execute script.
						It will handle input arguments, add a help function and process options.
						If you prefer keeping complete control you may not include this script, but you should try to replicate its behaviour with the options (for instance, the GUI runs <span class="code">execute.sh -d</span> to get the description of the method, and does not expect to actually run the method).
					</p>
					<p>
						The template produces the INPUT_GRAPH variable, a path to the input graph in an edgelist format.
						It also creates the OUTPUT_CLUST variable, that is the path to the output file that the script should produce.
						The format of these files are described <a href="use.php#formats">there</a>.
					</p>
					<p>
						If the input and/or output format of your program do not correspond to the internal format of CoDACom, a few scripts are available for format conversion in TOOLS_DIR.
						If those do not match your needs, you may contact us so that we can extend CoDACom to be more useful.
					</p>

					<p>
						Note that the <span class="file">methods/scrap_web.sh</span> script enables us to get implementations of community algorithms from the web without distributing them.
						If you want to distribute your code in that way, you can take a look at the script itself.
					</p>

					<h2 id="quality">Add a quality function/comparison method</h2>
					
					<p>
						Since quality functions are in the large majority simpler than community detection algorithms, we used a more standard OOP-centric approach for extension.
						To create a new quality function, one will need to :
					</p>
					<ul>
						<li>implement it in a C++ class (using the <a href="http://igraph.org/c/doc/">igraph</a> library)</li>
						<li>reference it in the Quality Function Provider (QFProvider)</li>
						<li>add it to the build list</li>
					</ul>
					
					<h3>Implementation</h3>
					<p>
						The working directory is, by default, <span class="file">src/libquality</span>.
						All the Quality Functions (QFs) are available in the <span class="file">quality/</span> subfolder.
						The process of creating a new QF will vary depending on its scope :
					</p>
					<ul>
						<li>A global QF is the most straightforward. One only need to extend AbstractQF, and fill in the constructor (by giving a name and a description), and two functions.
							The first, "apply", is the main function.
							It imputs a graph and a related membership vector, where keys are vertex ids and values are cluster ids.
							The second, "single_apply", is optionnal (return -1 if it is not relevant).</li>
						<li>A Single-Cluster Quality Function (SCQF) is implemented by extending AbstractSCQF.
							Only the constructor and the apply function need to be overwritten.
							The apply function recieves as input, on top of the previous ones, the cluster on which to do the computation as well as its id.</li>
						<li>A Single-Vertex Quality Function (SVQF) is implemented by extending AbstarctSVQF.
							One again, only the constructor and the apply function need rewriting.</li>
					</ul>
					
					<h3>Referencing</h3>
					<p>
						Once the new class is done, one need to reference it in the provider (the class QFProvider is available in <span class="file">src/libquality</span>).
						The constructor of QFProvider instanciates an object from all QF classes.
						If a SCQF was made, one need also to wrap it with an AgglomerativeQF, <i>e. g.</i> MeanQF or SumQF, that respectively take the mean and the sum of the cluster's value as the global quality.
						It it is a SVQF, the default behaviour is simply to take the average over all nodes.
						<strong>Warning : </strong> by default, the SVQFs are sampled, the number of samples can be set in AbstractSVQF.hpp.
					</p>
					
					<h3>Building</h3>
					<p>
						In order to make the class available as a binary, it needs to be be referenced by the Automake build system.
						Two files have to be updated (in <span class="file">src/libquality</span>) : <span class="file">targets_libquality</span> (referencing the .cpp files) and <span class="file">headers_libquality</span> (referencing the .hpp files).
					</p>

					<h3>Comparison methods</h3>
					<p>
						Comparison methods follow the same structure, with the notable exception that multiple scopes do not exist.
						All the corresponding files may be found in <span class="file">src/libcomparison</span>.
					</p>
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
