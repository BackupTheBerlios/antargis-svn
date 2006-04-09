<?

include("config/news.inc.php");

function view()
{
	$b=h1("News").getNews();

	return menuSystem($b);
}

?>
