<?
function makeMenu()
{
	return 
		menu("","","start/view.inc.php",
			array(
				menu("news","News","news/view.inc.php"),
				menu("screenshots","Screenshots","screenshots/view.inc.php"),
				menu("contact","Contact/Impressum","contact/view.inc.php")));
}

?>