function test_function() { 
	var match = 0;
	if(arguments[0] == arguments[1]) { 
	match = 1; 
	} 
	return match; 
}

function JSrepeat(name, repeat) { 
	var printthis = "";
	for(var i=0; i < repeat; i++){
		printthis += name;
	}
	return printthis; 
}

function ReturnThis(anything) { 
	return anything; 
}

function WhatsMyName() { 
	return "Jimmy"; 
}
