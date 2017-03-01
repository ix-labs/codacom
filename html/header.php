<div class="container" id="header"> 
  <h1 class="title">CoDACom</h1>
  <nav class="navbar navbar-default">
    <div class="container-fluid">     
      <!--div id="navbar" class="navbar-collapse collapse"-->
        <ul class="nav navbar-nav">
          <?php
					function head($link, $current_file, $name) {
						if($link == $current_file)
							echo "<li class=\"active\"><a href=\"$link\">$name</a></li>";
						else
							echo "<li><a href=\"$link\">$name</a></li>";
					}
					head("index.php", $file, "Home");
					head("install.php", $file, "Install");
					head("use.php", $file, "How to use");
					head("expand.php", $file, "How to extend");
					head("contact.php", $file, "Contact");
					head("demo.php", $file, "Demo");
					head("licence.php", $file, "Licence");
					?>
        </ul>
    </div><!--/.container-fluid -->
  </nav>
</div>
