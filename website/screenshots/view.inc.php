<?

function getScreenies()
{
	$screenies=array(
		array("Version 0.1.2 (06-05-16)",array(
			array("screenies/shot060421","A hero considering to invent."),
			array("screenies/shot060421a","Some fish."),
			array("screenies/shot060421b","Row, row, row your boat..."),
		)),
		array("Before first release (pre 0.1):",array(
			"screenies/shot051024b",
			array("screenies/shot051118","Some very old high-poly trees with apples."),
			array("screenies/shot060222","The campaign selection screen."),
			array("screenies/shot060222b","Some story is told..."),
			array("screenies/shot060222c","The first tutorial island."),
			array("screenies/shot060222d","In the north of the first tutorial Island.")
		)),
		array("Old isometric engine",array(
			"screenies/screenshot1",
			"screenies/shot050228",
			array("screenies/shot050304","Trees are generated and rendered with a voxel/ray-casting technique."),
			array("screenies/050511","Old 2.5d-engine. Graphics were generated using voxels and ray-casting. I wasted a lot of time on this. The Gui engine is already usable."),
		)),
		array("Very old plain 2d-engine",array(
			array("screenies/screenshot_veryold2d","A very old screenshot. This is one of the very first attempts.")
		))
	);
	return $screenies;
}

function getDesc($s)
{
	$x=array("screenies/shot060222"=>"Campaign selection screen");
	return $x[$s];
}

function smallPics()
{
	$screenies=getScreenies();
	
	$r="";
	
	$page="index.php?page=screenshots";
	$id=0;
	
	foreach($screenies as $kate)
	{
		$kat=$kate[0];
		$l=$kate[1];
		$r.="<h2>$kat</h2>\n";
		foreach($l as $a)
		{
			if(is_array($a))
			{
				$b=$a[0];
				$c=$a[1];
			}
			else
				$b=$a;
			//$ref=$page."&screenshotid=$id";
			$ref="$b.jpg";

			$ref="showpic.php?image=$ref&text=$c";
			$img=image($b."_thumb.jpg","screenshot");
			//$r.=anchor($page."&screenshotid=$id",image($b."_thumb.jpg","screenshot"));
			$r.="<a style='text-decoration:none;' href='$ref' target='_new'>$img</a>";
	
			$id++;
		}
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
		$comment="First tutorial island";
		if($sid>=0 && $sid<sizeof($screenies))
		{
			$a=$screenies[$sid];
			$comment=$a[1];
			if(is_array($a))
				$c=$a[0];
			else
			{
				$c=$a;
				$comment="No comment here.";
			}
			$b.=div("<div>$comment</div>".image($c.".jpg"),"screenshot");
		}
	}
	return menuSystem($b,"screenshotBody");
}
?>
