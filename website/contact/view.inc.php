<?

function view()
{
	$b=h1("Contact");
	
	$list=anchor("http://lists.berlios.de/mailman/listinfo/antargis-dev","http://lists.berlios.de/mailman/listinfo/antargis-dev");
	
	$b.=h3("Please contact us per mailing-list:");
	$b.=p("<a href='mailto:antargis-dev@lists.berlios.de'>antargis-dev@lists.berlios.de</a>");
	$b.=p("Subscribe to list here : $list");

	$b.=h3("Or on IRC:");
	$b.=p("Go to <a href='irc://irc.freenode.net'>irc.freenode.net</a> and join channel #antargis. Might be that someone's there.");

	$b.=p("&nbsp;");
	$b.=p("Information on who the Battles of Antargis team is can be found here: <a href='http://antargis.berlios.de/wiki/index.php/People'>People</a>.");
	$b.="<br><br>\n";
	$b.=p("David Kamphausen david dot kamphausen at web dot de");
	$b.=p("Oststraße 31a, 42277 Wuppertal");
	
	return menuSystem($b);
}

?>
