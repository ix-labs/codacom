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
		
		<?php $file='install.php'; include_once('header.php');?>
    
		<div class="container">
			<div class="row">
				
				<div class="col-xs-hidden col-sm-3">
					<nav id="leftCol">
						<ul class="nav nav-pills nav-stacked" id="sidebar">
						</ul>
					</nav>
				</div><!--/left-->
				
	      <div class="col-sm-9">
					<h2 id="download">Download</h2>
					
					<p>
						The current version of CoDACom may be downloaded <a href="codacom-current.tar.gz">here</a>.
					</p>
					
					<h3>Version</h3>
					
					<p>
					The current version number is 0.1.1.
					</p>
					
	      	<h2 id="dependancies">Dependancies</h2>
	      	
					<ul>
					<li>igraph</li>
					<li>expect (optionnal)</li>
					<li>mcl (optionnal)</li>
					<li>boost graph library (optionnal)</li>
					</ul>

					<p>
						CoDACom heavily depends on the <a href="http://igraph.org/">igraph</a> library.
						Fortunatly, this library is available through multiple repositories.
						Optional packages may be required for community detection algorithms implementation.
						If these packages are not installed, the corresponding algorithms will simply not be run.	      	
	      	</p>
						
	      	<h3 id="debian">Debian-based</h3>

					<p>Checked on Debian 8.2 from a fresh install.</p>
					<p>Required:</p>
					<div class="code">
					sudo apt-get install build-essential libigraph0-dev
					</div>
					<p>Optionnal:</p>
					<div class="code">
					apt-get install mcl libboost-graph-dev expect
					</div>
	      	
	      	<h3 id="OSX">OSX with brew</h3>
	      	
	      	<p>Checked on Yosemite 10.10.1 from a fresh install</p>
	      	<p>Required:</p>
					<div class="code">
						brew install homebrew/science/igraph <br />
						brew install python3
					</div>
					<p>Optionnal:</p>
					<div class="code">
						brew install boost <br />
						brew install mcl
					</div>

					<h3 id="generic">Generic Unix</h3>
	      	
					<p>If the version of igraph in your package manager is too old, you may install it from the <a href="http://igraph.org/c/#downloads">source</a>.
						If you install libraries in non-standard locations (<i>e. g.</i> using <span class="code">--prefix=path/to/igraph/install</span> as an option of the igraph configure step), you must inform the path of the binaries and of the include files to CoDACom.
						You may refer to these locations in two ways :</p>
					<ul>
						<li>either provide the locations as options of your configure, such as <span class="code">./configure LDFLAGS=-L/path/to/igraph/install/lib CPPFLAGS=-I/path/to/igraph/install/include</span>,</li>
						<li>or add the include location to your CPATH variable and the library location to your LD_LIBRARY_PATH variable</li>
					</ul>

					<h2 id="install">Installation</h2>

					<p>
						The package follows a standard autoconf installation procedure, but with a few twists.
						After uncompressing the archive, you may install it in the wollowing way :
					</p>
					<div class="code">
						cd methods/; sh scrap_web.sh; cd - <span class="comment">#Optionnal, import some methods from websites</span> <br />
						./configure <br />
						make <br />
						sh update_whitelist.sh <span class="comment">#Sets whitelists to the default ones</span>
					</div>
					<p>Since it is not a library, <span class="code"> make install</span> is not required.</p>
					
					<h2 id="server">Website (with GUI)</h2>

					<p>
						Before reading this section, it is advised to read at least the <a href="use.php">use guide</a>.
						The website uses PHP5 as its only prerequiste.
					</p>
					<p>
						In the <span class="file">html</span> folder of the archive, you will find the website you are currently browsing.
						In order for it to communicate with the CoDACom implementation, you need to provide the appropriate folders to two configuration files.
					</p>
					<ul>
						<li><span class="file">conf.php</span> is the server-side configuration of the website. It handles different behavioral aspects, and also references paths that are essential for the GUI.</li>
						<li><span class="file">vars.sh</span> (or any file named by the <span class="code">$vars_path</span> variable in <span class="file">conf.php</span>) is the configuration file that will be provided to the command-line interface.</li>
					</ul>
						

					<p>
						Usually, one would first install the command-line interface using the previous sections, in a folder we will call <span class="file">main</span>.
						Then, he would copy the content of the html folder into the repository used by the web server, that we will call <span class="file">web</span>.
						Afterwards, he would modify <span class="file">web/conf.php</span>, to provide the location of the <span class="file">main</span> directory to the <span class="code">$root_dir</span> variable.
						He would also make sure that the folders pointed to by the variables <span class="code">$gt_dir</span>, <span class="code">$rgt_dir</span>, <span class="code">$clusterings_dir</span> and <span class="code">$result_dir</span> can be listed by the web interface.
						He may finally let users upload graphs by setting the <span class="code">$authorise_upload</span> variable.
					</p>
					<p>
						If the shared mode is not used (<i>i.e.</i> the data is user-specific), the files in <span class="file">data/template</span> also need to be configured.
						A mirror of <span class="file">vars.sh</span> and <span class="file">conf.php</span> that will be copied for each user are there, and have to be edited.
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
