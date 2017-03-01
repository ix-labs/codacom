<?php
include_once("conf.php");
if(!$shared) {
	if(isset($_SESSION)) {
		if(isset($_COOKIE["codacom"])) {
			$_SESSION["codacom"] = $_COOKIE["codacom"];
		}
		if(!isset($_SESSION["codacom"]) or !is_dir($data_dir . "/" . $_SESSION["codacom"])) {
			//Create folders, sets unique id as 6-numbers string
			$id = new_session();
			$_SESSION["codacom"] = $id;
		}
		if(!isset($_COOKIE["codacom"])) {
			setcookie("codacom", $id);
		}
		include_once($data_dir . "/" . $_SESSION["codacom"] . "/session_conf.php");
	}
}

$wl_array = array(
  "raw" => "-r",
		"inputs" => "-i",
		"methods" => "-m",
		"quality" => "-q",
		"comparison" => "-c"
);

function new_session() {
	global $data_dir;
	$files = scandir($data_dir);
	$i = 0;
	$id = "000000";
	$nb = 0;
	while($i < count($files)) {
		$f = $data_dir . "/" . $files[$i];
		if(is_dir($f) and $files[$i][0] == "0") {
			$temp = intval($files[$i]);
			print($temp);
			if($temp >= $nb) {
				$nb = $temp + 1;
				$id = sprintf("%06s", $nb);
			}
		}
		$i++;
	}
	$rel_local_dir = $data_dir . "/" . $id;
	$abs_local_dir = getcwd() . "/" . $rel_local_dir;
	exec_print_err("cp -r ". $data_dir . "/template " . $rel_local_dir);
	exec_print_err("sed -i s:@@LOCAL_DIR@@:" . $abs_local_dir . ":g " . $abs_local_dir . "/vars.sh");
	exec_print_err("sed -i s:@@REL_LOCAL_DIR@@:" . $rel_local_dir . ":g " . $abs_local_dir . "/session_conf.php");
	exec_print_err("sed -i s:@@ABS_LOCAL_DIR@@:" . $abs_local_dir . ":g " . $abs_local_dir . "/session_conf.php");
	return $id;
}

function exec_print_err($cmd, &$result = array()) {
	$ret = 0;
	exec($cmd, $result, $ret);
	if($ret != 0) {
		echo "Error executing the command " . $cmd . ", error message is : \n";
		echo "/******************************/\n";
		echo implode("\n", $result);
		echo "\n/******************************/\n";
		echo "Return status : " . $ret . "\n";
	}
	return $ret;
}

//Returns true if the lock was successfull, false otherwise
function lock($process) {
	exec_print_err("touch process");
	$arr = array();
	$ret = 0 ;
	exec_print_err("ln -s process lock", $arr, $ret);
	if($ret == 0) {
		return 1;
	}
	return 0;
}

function unlock() {
	exec_print_err("rm -f lock");
}

function is_locked() {
	return file_exists("lock");
}

function get_locking_process() {
	$process = "";
	if(is_readable("process") && filesize("process") > 0) {
		$myfile = fopen("process", "r") or die("Unable to open file!");
		$process = trim(fread($myfile, filesize("process")), " \n");
		fclose($myfile);
	}
	return $process;
}

function list_available($what) {
	global $wl_array, $root_dir, $vars_path;
	$arr = array();
	if(array_key_exists($what, $wl_array)) {
		exec_print_err("cd ". $root_dir ." && /bin/sh update_whitelist.sh -l ". $wl_array[$what] . " -v " . $vars_path, $arr);
		return $arr;
	} else {
		return "Error : whitelist not recognised";
	}	
}

function gt_exists($what, $file) {
	global $root_dir, $rgt_dir, $gt_dir;
	if($what == "methods" or $what == "quality" or $what == "comparison")
		return false;
	$filename = pathinfo($file, PATHINFO_FILENAME); //Returns the file without extension
	$path = "";
	if($what == "raw")
		$path = $rgt_dir;
	else
		$path = $gt_dir;

	return file_exists($path . "/" . $filename);
}

//Returns an array (key is object, value is description) of the description that have been found
//$what arg describes what is being listed, the $which is an array of available objects
function get_descriptions($what, $which) {
	global $root_dir, $tools_dir, $methods_dir;
	$res = array();
	if($what == "quality" || $what == "comparison") {
		$arr = array();
		$exec_name = "";
		if($what == "quality")
			$exec_name = "list_qf";
		else
			$exec_name = "list_cm";
		exec_print_err($tools_dir . "/" . $exec_name . " -d", $arr);
		
		foreach($arr as $e) {
			$obj = substr($e, 0, strpos($e, ' '));
			$des = substr($e, strpos($e, ' ') + 1);
			$res[$obj] = $des;
		}
	}
	if($what == "methods") {
		foreach($which as $e) {
			$path = $methods_dir . "/" . $e;
			if(file_exists($path . "/execute.sh")) {
				$arr = array();
				exec_print_err("cd " . $path . " && /bin/sh execute.sh -d", $arr);
				$res[$e] = implode("\n", $arr);
			}
		}
	}
	return $res;
}

function update_whitelist($what, $list) {
	global $wl_array, $root_dir, $vars_path;
	$arr = array();
	$list_string = "";
	foreach($list as $e)
		$list_string = $list_string.$e." "; 
	if(array_key_exists($what, $wl_array)) {
		exec_print_err("cd ". $root_dir ." && /bin/sh update_whitelist.sh ". $wl_array[$what] ." -d -v ". $vars_path . " " . $list_string, $arr);
		return $arr;
	} else {
		return "Error : whitelist not recognised";
	}
}

//Updates whitelists from a JSON object containing the 5 arrays
function update_whitelists_JSON($json_whitelist) {
	update_whitelist("raw", $json_whitelist->{"raw"});
	update_whitelist("inputs", $json_whitelist->{"inputs"});
	update_whitelist("methods", $json_whitelist->{"methods"});
	update_whitelist("comparison", $json_whitelist->{"comparison"});
	update_whitelist("quality", $json_whitelist->{"quality"});
}

function execute($process, $option, $whitelist) {
	global $root_dir, $vars_path;
	#Change the var.sh
	update_whitelists_JSON(json_decode($whitelist));
	#Change the current process
	exec_print_err("echo ".$process." > process;");
	#Exec the thing
	exec_print_err("cd ". $root_dir ." && /bin/sh execute.sh ". $option ." ". $vars_path ." 2>&1", $arr);
}

function execute_preprocessing($whitelist) {
	execute("preprocessing", "-p", $whitelist);
}

function execute_methods($whitelist) {
	execute("methods", "-m", $whitelist);
}

function execute_postprocess($whitelist, $subproc) {
	global $root_dir, $tools_dir, $vars_path;
	$analysis = true;
	$procs = array(
		"comparison" => "A_CORRELATION",
			"quality" => "A_QUALITY",
			"visu" => "C_VISUALISATION",
			"tex" => "C_TEX",
			"quotient" => "C_QUOTIENT");

	foreach ($procs as $k => $v) {
		$value = 0;
		if ($k == $subproc) {
			if ($v[0] != "A")
				$analysis = false;
			$value = 1;
		}
		//echo "sh " . $tools_dir . "/change_var.sh " . $root_dir . "/vars.sh " . $v . " " . $value;
		exec_print_err("sh " . $tools_dir . "/change_var.sh " . $vars_path . " " . $v . " " . $value);
	}
	if($analysis)
		execute($subproc, "-a", $whitelist);
	else
		execute($subproc, "-c", $whitelist);
}

function upload_graph($tmp_name, $name) {
	global $raw_dir;
	move_uploaded_file($tmp_name, "$raw_dir/inputs/$name");
}

function upload_gt($tmp_name, $name) {
	global $raw_dir;
	move_uploaded_file($tmp_name, "$raw_dir/ground_truth/$name");
}

?>
