/*Sidebar handling*/

$(document).ready(function() {       
	/* Dynamic generation of the sidebar */
	var bar = document.getElementById("sidebar");
	if(bar != null) { /* If there is such an element as a sidebar */
		var titles = document.getElementsByTagName("H2");
		for(var i = 0; i < titles.length; i++) {
			var node = document.createElement("li");
			var child = document.createElement("a");
			child.href = "#" + titles[i].id;
			child.textContent = titles[i].textContent;
			node.appendChild(child);
			bar.appendChild(node);
		}
	}

	//activate sidebar
	$('#sidebar').affix({
	  offset: {
	    top: $('#header').height()
	  }
	});
	
	//activate scrollspy menu
	var $body   = $(document.body);
	//var navHeight = 0;
	var navHeight = $('.navbar').outerHeight(true) + 10;
	
	$body.scrollspy({
		target: '#leftCol',
		offset: $('#header').height()
	});
	
	//smooth scrolling sections
	$('a[href*=#]:not([href=#])').click(function() {
	    if (location.pathname.replace(/^\//,'') == this.pathname.replace(/^\//,'') && location.hostname == this.hostname) {
	      var target = $(this.hash);
	      target = target.length ? target : $('[name=' + this.hash.slice(1) +']');
	      if (target.length) {
	        $('html,body').animate({
	          scrollTop: target.offset().top - 50
	        }, 1000);
	        return false;
	      }
	    }
	});
});    

