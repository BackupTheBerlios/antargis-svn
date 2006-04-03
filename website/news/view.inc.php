<?

function news($date,$title,$body="")
{
	return h3($date." - ".$title).p($body);
}

function view()
{
	$b=h1("News");

	$b.=news("2006-04-03","Help anouncement","This weekend I filed a help wanted message at the Linux Game Tome, but this little impact yet. Maybe it's to come after a first appearance on the front page, as I have submitted it as a new game.");
	$b.=news("2006-03-23","Forum","Come to the forum and discuss the game's idea, screenshots and whatever. I'll very soon anounce the first tech-demo, which will hopefully gain some interest and maybe gather some developers and artits to join!");
	$b.=news("2005-12-08","Music","I've made a first attempt to create some music: <a href='http://antargis.berlios.de/wiki/index.php/Music'>music in wiki</a>");
	$b.=news("2005-12-07","Wiki !","A (development-)wiki has been added. <a href='wiki/'>go to wiki</a>");
	
	$f="<ul>
		<li>complete rewrite of rendering and switch to 3d !!</li>
		<li>some of the game logic is already there. You can already fight and conquer castles.</li>
		<li>men run around fetch fruit, stone and wood and produce tools in the workshop</li>
		<li>a very rudimentary AI is implemented, but not really working yet.</li>
	</ul>";
	$b.=news("2005-11-22","Great progress","News writing these days is somewhat useless as I've done no promotion yet. But behind the scene there is pretty much new:$f");
	$b.=news("2005-04-12","Homepage update","Martin Kamphausen provided some nice logo. AntargisGUI-lib is proceeding - XML-support for layouting is included now.");
	$b.=news("2005-04-05","Homepage setup","Welcome to to first version of this website!<br>There is nothing much here at the moment, but it is soon to come.");


	return menuSystem($b);
}

?>
