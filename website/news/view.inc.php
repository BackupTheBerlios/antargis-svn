<?

function news($date,$title,$body="")
{
	return h3($date." - ".$title).p($body);
}

function view()
{
	$b=h1("News");
	$b.=news("2005-04-05","Homepage setup");


	return menuSystem($b);
}

?>