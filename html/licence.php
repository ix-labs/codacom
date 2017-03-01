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
		<?php $file='licence.php'; include_once('header.php');?>
		
		<p>
			The top image is from <a href="https://xkcd.com/802/">xkcd</a>.
			It was cropped, the luminosity has been highened and some cropped text was removed.
			Unlike the rest of CoDACom, this image is under <a href="http://creativecommons.org/licenses/by-nc/2.5/">Creative commons Attribution-NonCommercial 2.5</a>, therefore you may not use it for commercial purposes.
		</p>

		<p>
			The rest of the software is under the <a href="http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html">CeCILL-B</a> licence, a french equivalent to the BSD.
			It means that you are allowed to copy and modify the code without any restriction, and that you may share the (possibily modified) source code if you specify the original source.
		</p>
		
		<p>
			Please note that the software will download code under different licence if the related script is run.
			As for now, it downloads two methods :
		</p>
		<ul>
			<li>The effecient implementation of the <a href="http://sourceforge.net/projects/louvain/">louvain algorithm</a></li>
			<li>The java implementation of the <a href="http://www.emilio.ferrara.name/code/conclude/">conclude algorithm</a></li>
		</ul>

	</body>
</html>
