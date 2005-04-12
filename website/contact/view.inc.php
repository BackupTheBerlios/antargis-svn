<?

function view()
{
	$b="";
	
	$list=anchor("http://lists.berlios.de/mailman/listinfo/antargis-dev","http://lists.berlios.de/mailman/listinfo/antargis-dev");
	
	$b.=h3("Please contact us per mailing-list:");
	$b.=p("antargis-dev@lists.berlios.de");
	$b.=p("Subscribe to list here : $list");
	
	$b.=h3("Design/Development:");
	$b.=p("David Kamphausen david dot kamphausen at web dot de");
	$b.=p("Oststraße 31a, 42277 Wuppertal");
	$b.=h3("Webdesign:");
	$b.=p("Martin Kamphausen");
	$b.=p("David Kamphausen");
	
	return menuSystem($b);
}

?>