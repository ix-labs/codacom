<?php 
session_start();
include_once("model.php");
?>
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
		<script>
		var tooltips = <?php if($tooltips) echo "true"; else echo "false"?>;
		</script>
  </head>
  <body>
		<?php $file='demo.php'; include_once('header.php'); ?>

		<!--?php phpinfo(); ?-->

		<div class="container">
			<p>
				This is a demo presenting some features of CoDACom.
				Two graphes may be analysed : Zachary's karate club and the american college football network, both may be found on <a href="http://www-personal.umich.edu/~mejn/netdata/">M. E. J. Newman</a> personnal page.
			</p>
			<div class="jumbotron" style="background-color:#a7eeba" id="server-availability">
				
			</div>

      <div class="row">
				<div class="col-md-12 col-lg-1">
					<a class="btn btn-default" href="javascript:void(0);" onclick="javascript:checkAll(false);" role="button">Unselect</a>
					<a class="btn btn-default" href="javascript:void(0);" onclick="javascript:checkAll(true);" role="button">Select</a>
					<a class="btn btn-default" href="javascript:void(0);" onclick="javascript:checkTheseBoxes(new Array('<?php echo implode("','", $linear_algorithms);?>'));" role="button">Linear</a>
					<a class="btn btn-default" href="<?php echo $log_path; ?>" role="button" target="_blank">Log</a>
				</div>
				<div class="computation-block col-xs-12 col-sm-5 col-lg-2">
					
					<?php if(!$authorise_upload) { ?>
						<img class="disabled-img" src="img/grey_stripes_cropped.png" alt="upload has been disabled" data-toggle="tooltip" title="Upload has been disabled by server configuration">
					<?php } ?>
					<h3>1. Preprocessing</h3>
					<h4>Upload graph</h4>
					<div id="up_graph_list"></div>
					<form action="demo-handler.php" method="post" enctype="multipart/form-data">
						<div class="form-group">
							<label class="btn btn-default">
								<input type="file"  id="upload_graph" name="upload_graph[]" multiple="multiple" onchange="javascript:write_selected_files('up_graph_list', 'upload_graph')">Graph file
							</label>
							<button type="submit" id="sub_upload_graph" class="btn btn-default" name="sub_upload_graph">Upload</button>
						</div>
					</form>

					<div id="up_gt_list"></div>
					<form action="demo-handler.php" method="post" enctype="multipart/form-data">
						<div class="form-group">
							<label class="btn btn-default">
								<input type="file"  id="upload_gt" name="upload_gt[]" multiple="multiple" onchange="javascript:write_selected_files('up_gt_list', 'upload_gt')">Ground truth
							</label>
							<button type="submit" id="sub_upload_gt" class="btn btn-default" name="sub_upload_gt">Upload</button>
						</div>
					</form>

					<h4>Format conversion</h4>
					<form>
						<div class="list_scroll" id="raw_list">
						</div>
						<button type="button" onclick="javascript:request('list', 'raw', raw_list_callback);"  class="btn btn-default" aria-label="Refresh">
							<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
						</button>
						<!-- <button type="button" href="javascript:void(0);" id="sub_preprocess" onclick="javascript:request('preprocessing', 'none', processing_callback);" class="btn btn-default">Preprocess</button> -->
						<button type="button" id="sub_preprocess" onclick="javascript:request_preprocessing();" class="btn btn-default">Preprocess</button>
						<a class="btn btn-default" href="<?php echo $preproc_dir ?>" role="button" target="_blank">Results</a>
					</form>
				</div>

				<!-- Second block -->
				<div class="col-xs-12 col-sm-7 col-lg-3">
					<!--  Down arrow appearing appearing in xs -->
					<div class="col-xs-12 hidden-sm hidden-md hidden-lg">
						<span class="center-block glyphicon big-icon glyphicon-circle-arrow-down"></span>
					</div>

					<div class="row">
						<!--  Right arrow appearing appearing in sm and bigger -->
						<div class="hidden-xs col-sm-4 col-lg-4 vcenter">
							<span class="center-block glyphicon big-icon glyphicon-circle-arrow-right"></span>
						</div><!--
										 --><div class="computation-block col-xs-12 col-sm-8 col-lg-8 vcenter">
							<h3>2. Methods</h3>
							<h4>Select method inputs</h4>
							<form id="form_methods">
								<div class="list_scroll" id="inputs_list">
								</div>

								<h4>Select methods</h4>
								<div class="list_scroll" id="methods_list">
								</div>
								<button type="button" onclick="javascript:request('list', 'methods', methods_list_callback);request('list', 'inputs', inputs_list_callback);"  class="btn btn-default" aria-label="Refresh">
									<span class="glyphicon glyphicon-refresh" aria-hidden="true"></span>
								</button>
								<button type="button" id="sub_methods" onclick="javascript:request_methods();" class="btn btn-default">Execute</button>
								<a class="btn btn-default" href="<?php echo $clusterings_dir ?>" role="button" target="_blank">Results</a>
							</form>
										 </div>
					</div>
				</div>

				<!-- Vertical arrow pointing down when in sm -->
				<div class="col-sm-7 hidden-xs hidden-lg"></div>
				<div class="hidden-xs col-sm-5 hidden-lg">
					<span class="center-block glyphicon big-icon glyphicon-circle-arrow-down"></span>
				</div>
				<!-- End vertical arrow -->

				<!-- Third block-->
				<div class="col-xs-12 col-sm-7 col-lg-3 col-sm-push-5 col-lg-push-0">
					<!--  Down arrow appearing in xs -->
					<div class="col-xs-12 hidden-sm hidden-md hidden-lg">
						<span class="center-block glyphicon big-icon glyphicon-circle-arrow-down"></span>
					</div>
					<div class="row">
						<!--  Left arrow appearing in sm and md -->
						<div class="col-sm-4 hidden-xs hidden-lg vcenter">
							<span class="center-block glyphicon big-icon glyphicon-circle-arrow-left"></span>
						</div><!--
										 --><div class="hidden-xs hidden-sm hidden-md col-lg-4 vcenter"> <!--  Right arrow appearing in lg and bigger -->
							<span class="center-block glyphicon big-icon glyphicon-circle-arrow-right"></span>
										 </div><!--
															--><div class="computation-block col-xs-12 col-sm-8 vcenter">
											 <h3>3. Analysis</h3>

											 <h4>Clustering correlation</h4>
											 <form>
												 <div class="small_list_scroll" id="comparison_list">
												 </div>
												 <button type="button" id="sub_comp" onclick="javascript:request_postprocessing('comparison');" class="btn btn-default">Compare clusterings</button>
												 <a class="btn btn-default" href="<?php echo $result_dir ?>" role="button" target="_blank">Results</a>

												 <h4>Clustering quality</h4>
												 <div class="list_scroll" id="quality_list">
												 </div>
												 <button type="button" id="sub_quality" onclick="javascript:request_postprocessing('quality');" class="btn btn-default">Compute qualities</button>
												 <a class="btn btn-default" href="<?php echo $result_dir ?>/quality" role="button" target="_blank">Results</a>
											 </form>
															</div>
					</div> 
				</div>
				
				<!-- Fourth block -->
				<div class="col-xs-12 col-sm-5 col-lg-3 col-sm-pull-7 col-lg-pull-0">
					<!--  Down arrow appearing in xs -->
					<div class="col-xs-12 hidden-sm hidden-md hidden-lg">
						<span class="center-block glyphicon big-icon glyphicon-circle-arrow-down"></span>
					</div>
					<div class="row">
						<!--  Right arrow appearing in lg -->
						<div class="col-lg-4 hidden-md hidden-sm hidden-xs vcenter">
							<span class="center-block glyphicon big-icon glyphicon-circle-arrow-right"></span>
						</div><!--
										 --><div class="computation-block col-xs-12 col-lg-8 vcenter">
							<h3>4. Conversion</h3>
							
							<h4>Visualisation</h4>
							<form>
								<button type="button" id="sub_visu" onclick="javascript:request_postprocessing('visu');" class="btn btn-default">Export</button>
								<a class="btn btn-default" href="<?php echo $result_dir ?>/visualisation" role="button" target="_blank">Results</a>
							</form>
							
							<h4>Convert to TeX</h4>
							<form>
								<button type="button" id="sub_tex" onclick="javascript:request_postprocessing('tex');" class="btn btn-default">Convert</button>
								<a class="btn btn-default" href="<?php echo $result_dir ?>/tex" role="button" target="_blank">Results</a>
							</form>
							
							<h4>Quotient graph</h4>
							<form>
								<button type="button" id="sub_quotient" onclick="javascript:request_postprocessing('quotient');" class="btn btn-default">Convert</button>
								<a class="btn btn-default" href="<?php echo $result_dir ?>/quotient" role="button" target="_blank">Results</a>
							</form>
										 </div>
					</div>
				</div>
			</div>
		</div>
		
		
		<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
		<script src="jquery.min.js"></script>
		<!-- Include specific JS for this page -->
		<script src="demo.js"></script>
		<!-- Include all compiled plugins (below), or include individual files as needed -->
		<script src="bootstrap/js/bootstrap.min.js"></script>
		
		<script type="text/javascript">
		request('list', 'raw', raw_list_callback);
		request('list', 'inputs', inputs_list_callback);
		request('list', 'methods', methods_list_callback);
		request('list', 'quality', quality_list_callback);
		request('list', 'comparison', comparison_list_callback);
		request("availibility", "none", ping_callback);
		setInterval(function() {request("availibility", "none", ping_callback);}, <?php echo $update_time ?>);
		</script>
  </body>
</html>


