<?

function view()
{
	$body=h1("Welcome to Battles of Antargis");
	
	$body.=p("Battles of Antargis is a realtime strategy game. Its main purpose lies on conquering not on building.");
	
	$body.=p("For a better overview have a look at the screenshots.");
	
	
	
	
	return menuSystem($body);
	
}

?>