<?

function getScreenies()
{
	$screenies=array("screenies/screenshot1","screenies/shot050228","screenies/shot050304","screenies/050511",
	"screenies/shot051024b",
	"screenies/shot051118"
	
	);
	return $screenies;
}

function smallPics()
{
	$screenies=getScreenies();
	
	$r="";
	
	$page="index.php?page=screenshots";
	$id=0;
	
	foreach($screenies as $a)
	{
		$r.=anchor($page."&screenshotid=$id",image($a."_thumb.jpg","screenshot"));
		$id++;
	}
		
	return div($r,"thumbs");
}

function view()
{
	$sid=$_GET[screenshotid];
	//$b=image("screenies/screenshot1.png").image("screenies/shot050228.png").image("screenies/shot050304.jpg");

	$b="";
	$b.=h1("Screenshots");
	$b.=p("Click on small images for big screenshots.");
	$b.=smallPics();
	
	if($sid!="")
	{
		$screenies=getScreenies();
		if($sid>=0 && $sid<sizeof($screenies))
			$b.=p(image($screenies[$sid].".jpg"),"screenshot");
	}
	return menuSystem($b);
}
?>