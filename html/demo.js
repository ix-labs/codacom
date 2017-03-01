//Checks (or unchecks) all the checkbox of the document
function checkAll(state) {
  var boxes = new Array(); 
  boxes = document.getElementsByTagName('input');
 
  for (var i=0; i<boxes.length; i++)  {
    if (boxes[i].type == 'checkbox')   {
      boxes[i].checked = state;
    }
  }
}

//Does an array contains an object?
function contains(a, obj) {
    for (var i = 0; i < a.length; i++) {
        if (a[i] === obj) {
            return true;
        }
    }
    return false;
}

//Checks all of the checkboxes corresponding to linear algorithms
function checkTheseBoxes(names) {
  var boxes = new Array(); 
  boxes = document.getElementById("methods_list").getElementsByTagName('input');
 
  for (var i=0; i<boxes.length; i++)  {
    if (boxes[i].type == 'checkbox' && contains(names, boxes[i].parentElement.textContent.trim()))   {
      boxes[i].checked = true;
    }
  }
}

//Greys(b=true)/ungreys(b=false)  all of the computation buttons
function grey(b) {
	var to_grey = new Array("sub_upload_graph", "sub_upload_gt", "sub_preprocess", "sub_methods", "sub_comp", "sub_quality", "sub_visu", "sub_tex", "sub_quotient");
	for(var i=0; i<to_grey.length; i++)  {
		var btn = document.getElementById(to_grey[i]);
		btn.disabled = b;
	}
}

//Creates the XMLHttpRequest for Ajax
function getXMLHttpRequest() {
	var xhr = null;
	if (window.XMLHttpRequest || window.ActiveXObject) {
		if (window.ActiveXObject) {
			try {
				xhr = new ActiveXObject("Msxml2.XMLHTTP");
			} catch(e) {
				xhr = new ActiveXObject("Microsoft.XMLHTTP");
			}
		} else {
			xhr = new XMLHttpRequest(); 
		}
	} else {
		alert("Your browser does not support XMLHTTPRequest, which is required to use this interface");
		return;
	}
	return xhr;
}

//An ajax request to demo-handler.php, the callback takes a JSON object as input
function request(r, optional, callback) {
	var o = encodeURIComponent(optional); //Handles the spaces
	var xhr = getXMLHttpRequest();

	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
			//alert(xhr.responseText);
			var json = "";
			try {
        json = JSON.parse(xhr.responseText);
			} catch (e) {
				alert("Return not recognised as valid JSON : \n" + xhr.responseText);
        return false;
			}
			callback(json);
		} else if (xhr.readyState < 4) {
			//We could put a loading icon
		}
	};
	xhr.open("POST", "demo-handler.php", true);
	xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	xhr.send("request=" + r + "&optional=" + o);
}

//The callback of the "ping" request, asking the server whether some computation is happening
function ping_callback(s) {
	var a = s.available;
	var node = document.getElementById("server-availability");
	if(a == "true") {
		node.style = "background-color:#a7eeba";
		node.innerHTML = '<h3 class="center">The server is available</h3>';
		grey(false);
	} else {
		var p = s.process;
		node.style = "background-color:#ff9999";
		node.innerHTML = '<h3 class="center">The server is busy, current process is : ' + p + '</h3>';
		grey(true);
	}
}

//The callback of the preprocessing request
function processing_callback(s) {
	var a = s.success;
	if(a == "false") {
		alert("Computation in progress, please wait.");
	} else {
		alert("Computation done");
	}
}

//A common callback for list update
function list_callback(s, id) {
	var listHTML = document.getElementById(id);
	listHTML.innerHTML = "";
	var dv = document.createElement("div");
	for (i in s) {
		var file = s[i].name;
		var dv = document.createElement("div");
		dv.className = "checkbox-inline no_indent";
		var options = "";
		if(s[i].gt)
			options = 'class="blue" data-toggle="tooltip" title="This graph has a ground-truth"';
		else {
			if(s[i].hasOwnProperty("description") && tooltips) {
				options += ' data-toggle="tooltip" title="' + s[i].description + '"';
				//alert(options);
			}
		}
		dv.innerHTML = '<label ' + options + '><input type="checkbox" value="' + file + '">' + file + '</label>';
		listHTML.appendChild(dv);
	}
}

function raw_list_callback(s) {
	list_callback(s, "raw_list");
}

function inputs_list_callback(s) {
	list_callback(s, "inputs_list");
}

function methods_list_callback(s) {
	list_callback(s, "methods_list");
}

function quality_list_callback(s) {
	list_callback(s, "quality_list");
}

function comparison_list_callback(s) {
	list_callback(s, "comparison_list");
}

//A tool that loops over the children and find the selected ones. It outputs an array of their value
function list_selected_options(id) {
	var listHTML = document.getElementById(id);
	var listInputs = listHTML.getElementsByTagName("INPUT");
	var res = [];
	for (var i = 0; i < listInputs.length; i++) {
		var e = listInputs[i];
		if(e.checked) {
			res.push(e.value);
		}
	}
	return res;
}

//Outputs a JSON string of the selected whitelists
//The object has 5 keys : 
//raw, inputs, methods, comparison and quality
function json_whitelists() {
	return {
		raw: list_selected_options("raw_list"),
		inputs: list_selected_options("inputs_list"), 
		methods: list_selected_options("methods_list"),
		comparison: list_selected_options("comparison_list"), 
		quality: list_selected_options("quality_list")
	};
}

function request_preprocessing() {
	var whitelist = JSON.stringify(json_whitelists());
	request('preprocessing', whitelist, processing_callback);
}

function request_methods() {
	var whitelist = JSON.stringify(json_whitelists());
	request('methods', whitelist, processing_callback);
}

function request_postprocessing(name) {
	var json = json_whitelists();
	json.subproc = name;
	var whitelist = JSON.stringify(json);
	request('postprocess', whitelist, processing_callback);
}

/* Automatically shows the selected files from the file input */
function write_selected_files(list_id, button_id) {
	var list = document.getElementById(list_id);
	var files = document.getElementById(button_id).files;
	list.innerHTML = "";
	var str = "";
	if (typeof files !== "undefined") {
    for (var i=0, l=files.length; i<l; i++) {
			var dv = document.createElement("div");
			dv.innerHTML = files[i].name + '<br />';
			list.appendChild(dv);
    }
  }
  else {
    alert("Your browser does not accept multiple file selection");
  }   
}
