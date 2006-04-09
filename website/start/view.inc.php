<?

function screenshotPreview()
{
	$shot="screenies/shot060222d_thumb.jpg";

	$r=image($shot);

	$r="<div class='screenshotPreview'><a href='index.php?page=screenshots'>$r<br>More screenshots »</a></div>";

	return $r;
}

function view()
{
	$body=h1("Welcome to Battles of Antargis");
	
	/*$body.=p("Battles of Antargis is a realtime strategy game. Its main purpose lies on conquering not on building.");
	$body.=p("For a better overview have a look at the screenshots.");*/

	$desc=p("Battles of Antargis is a medieval realtime strategy game. You are a hero who was betrayed and you have to unify the kingdom and rebuild the old empire. Old myths will pass your way.");

	$desc.=p("The game is currently in heavy development and taking up pace. The current features include:
<ul>
	<li>A short tutorial giving you an introduction to the gameplay</li>
</ul>");


	$body.=screenshotPreview();
	$body.=$desc;

//Its main purpose lies on conquering not on building.");
	$body.=p("For a better overview have a look at the screenshots.");
	
	$body.=h3("Licensing/Copying");
	
	$GPL=anchor("GPL","http://www.gnu.org/copyleft/gpl.html");
	
	$body.=p("This game and its libraries are distributed under $GPL. So you are free to use it.");
	
	
	
	return menuSystem($body);
	
}

?>
