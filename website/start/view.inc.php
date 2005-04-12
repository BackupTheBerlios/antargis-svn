<?

function view()
{
	$body=h1("Welcome to Battles of Antargis");
	
	$body.=h3("Description");
	$body.=p("Battles of Antargis is a realtime strategy game. Its main purpose lies on conquering not on building.");
	$body.=p("For a better overview have a look at the screenshots.");
	
	$body.=h3("Licensing/Copying");
	
	$GPL=anchor("GPL","http://www.gnu.org/copyleft/gpl.html");
	
	$body.=p("This game and its libraries are distributed under $GPL. So you are free to use it.");
	
	
	
	
	return menuSystem($body);
	
}

?>