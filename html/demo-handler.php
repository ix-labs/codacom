<?php
session_start();
include_once("model.php");

#There is a risk of code injection since list get into options of shell commands
#We simply change ; with whitespaces, disabling the possibility of "getting out" of commands 
function escape($string) {
	return str_replace(";", " ", $string);
}

$locked_msg = "{\"success\":\"false\"}";
$success_msg = "{\"success\":\"true\"}";

/** Ajax call **/
$request = (isset($_POST["request"])) ? $_POST["request"] : NULL;
//JSON or text options, the content is different for each request
$optional = (isset($_POST["optional"])) ? $_POST["optional"] : NULL;

/** File upload call **/
$file = NULL;
$gt = false;
if(isset($_POST["sub_upload_graph"])) {
	$file = $_FILES["upload_graph"];
} else if(isset($_POST["sub_upload_gt"])){
	$file = $_FILES["upload_gt"];
	$gt = true;
}

if ($request) {
	if($request == "availibility") {
		if(is_locked()) {
			$process = get_locking_process();
			echo "{\"available\":\"false\", \"process\":\"" . $process . "\"}";
		} else {
			echo "{\"available\":\"true\"}";
		}
	} else if($request == "list") {
		$res = list_available($optional);
		if(is_array($res)) {
			$arr = array();
			$descriptions = get_descriptions($optional, $res);
			$response = "[";
			foreach($res as $e) {
				$response = $response . '{"name":"' . $e . '", "gt" : ';
				if(gt_exists($optional, $e))
					$response = $response . 'true';
				else
					$response = $response . 'false';
				if(array_key_exists($e, $descriptions)) {
					$response = $response . ', "description":"' . $descriptions[$e] . '"';
				}
				$response = $response . '},';
			}
			$response = rtrim($response, ",");
			$response = $response . ']';
			echo $response;
			//echo "[\"".implode("\",\"", $res)."\"]";
		} else {
			echo $res;
		}
	} else if ($request == "preprocessing") {
		if(lock("preprocessing")) {
			execute_preprocessing(escape($optional));
			unlock();
			echo $success_msg;
		} else
			echo $locked_msg;
	} else if ($request == "methods") {
		if(lock("methods")) {
			execute_methods(escape($optional));
			unlock();
			echo $success_msg;
		} else
		echo $locked_msg;
	} else if ($request == "postprocess") {
		if(lock("postprocess")) {
			$json = json_decode($optional, true);
			if (isset($json['subproc'])) {
				execute_postprocess($optional, $json['subproc']);
			} else
			echo "Error : subprocess not set in the Ajax postprocess request";
			unlock();
			echo $success_msg;
		} else
		echo $locked_msg;
	}	else {
		echo "Ajax request name not recognised";
	}
} else if($file && $authorise_upload) {
	$nb = count($file["name"]);
	$error = 0;
	$i = 0;
	while($i < $nb and !$error) {
		$name = $file["name"][$i];
		$tmp_name = $file["tmp_name"][$i];
		$err = $file["error"][$i];
		$size = $file["size"][$i];

		if($size > $max_size*1000) {
			$error = 1;
			echo "File ".$name." is too big";
		} else {
			for($j = 0; $j < count($forbidden_extensions); $j++) {
				$ext = $forbidden_extensions[$j];
				if(substr($name, -strlen($ext)) === $ext) { //Does the file ends with a forbidden extension?
					$error = 1;
					echo "File ".$name." has a forbidden extension";
				} else {
					if($gt) {
						upload_gt($tmp_name, $name);
					} else {
						upload_graph($tmp_name, $name);
					}
				}
			}
		}
		$i++;
	}
	if(!$error)
		header('Location: demo.php');
} else {
  echo "FAIL";
}


?>
