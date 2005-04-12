<?

function news($date,$title,$body="")
{
	return h3($date." - ".$title).p($body);
}

function view()
{
	$b=h1("News");
	$b.=news("2005-04-12","Homepage update","Martin Kamphausen provided some nice logo. AntargisGUI-lib is proceeding - XML-support for layouting is included now.");
	$b.=news("2005-04-05","Homepage setup","Welcome to to first version of this website!<br>There is nothing much here at the moment, but it is soon to come.");


	return menuSystem($b);
}

?>