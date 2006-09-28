<?

include ("model/news.inc.php");

function getCurrentNews($toponly)
{
	$newsids=getNewsIds();

	$ouput=array();
	foreach($newsids as $id)
	{
		$output[]=new NewsItem($id);
	}
	$output=array_reverse($output);
	if($toponly)
		$output=array_slice($output,0,3);
	return $output;
}

function getNews($toponly=false)
{
	return includeTemplate("news/view.thtml",array("output"=>getCurrentNews($toponly)));
}


?>
