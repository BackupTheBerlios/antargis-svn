<?
function makeMenu()
{
	return 
		menu("","","start/view.inc.php",
			array(
				menu("about","About","start/view.inc.php"),
				menu("news","News","news/view.inc.php"),
				menu("screenshots","Screenshots","screenshots/view.inc.php"),
				menu("features","Features","features/view.inc.php"),
				menu("download","Download","download/view.inc.php"),
				menu("contact","Contact/Impressum","contact/view.inc.php"),
				menu("http://antargis.berlios.de/wiki","Wiki...","")));
				menu("http://developer.berlios.de/projects/antargis","Project site...","")));
}

?>