<?
include("config/news.inc.php");

function getTopNews()
{
	echo getNews(true);
}
function view()
{
	return menuSystem(includeTemplate("start/view.thtml"));
}

?>
