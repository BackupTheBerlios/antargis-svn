<?

function news($date,$title,$body="")
{
	return h3($date." - ".$title).p($body);
}

function view()
{
	$b=h1("News");
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