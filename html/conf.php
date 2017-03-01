<?php
$root_dir = "/www/codacom/private/codacom-0.1";
$methods_dir = $root_dir . "/methods";
$tools_dir = $root_dir . "/src/tools";
$gt_dir = "preprocessed/ground_truth";
$preproc_dir = "preprocessed";
$rgt_dir = $root_dir . "/raw/ground_truth";
$clusterings_dir = "clusterings";
$result_dir = "results";
$log_path = "log";
$vars_path = getcwd() . "/vars.sh";

//Two possible behaviors : shared or not
//Shared data means that all the users have access by default to the same data
//Unshared data means that every user will have access to his own computed results through the interface
$shared = false;
//The data dir has to include a template dir that will be copied for each session
$data_dir = "data";

$linear_algorithms = array("mcl", "louvain", "lexdfs", "infomap", "scd");

$max_size = 1000; //Size max of file upload in KB
$forbidden_extensions = array(".php", ".htaccess"); //Forbidden extensions for uploaded files
$authorise_upload = false;

/* Modification of the client behaviour */
$update_time = 3000; //The time between each check of server availibility (in ms)
$tooltips = true;
