<?

class Screenshot
{
	var $thumb="";
	var $url="";
	function Screenshot($name,$text)
	{
		$this->thumb=$name."_thumb.jpg";
		$this->url="showpic.php?image=$name.jpg&text=$text";
	}

}

function getScreenies()
{
	$screenies=array(
		array("Additional included display for those without OpenGL-support (07-05-06)",array(
			new Screenshot("screenies/nogltest_0","Some test screenshots"),
			new Screenshot("screenies/nogltest_0","Some test screenshots"),
		)),
		array("Version 0.1.9 (06-09-20)",array(
			new Screenshot("screenies/village","You can build your own village now."),
			new Screenshot("screenies/new_terrain","Some new terrain textures."),
			new Screenshot("screenies/Ovejas","What happens when there is no wolf to eat the sheeps?"),
			new Screenshot("screenies/Antargis","A peaceful village in a peaceful island"),
			new Screenshot("screenies/Antargis2","No comment"),
		)),

		array("Version 0.1.2 (06-05-16)",array(
			new Screenshot("screenies/shot060421","A hero considering to invent."),
			new Screenshot("screenies/shot060421a","Some fish."),
			new Screenshot("screenies/shot060421b","Row, row, row your boat..."),
		)),
		array("Before first release (pre 0.1):",array(
			new Screenshot("screenies/shot051024b","-"),
			new Screenshot("screenies/shot051118","Some very old high-poly trees with apples."),
			new Screenshot("screenies/shot060222","The campaign selection screen."),
			new Screenshot("screenies/shot060222b","Some story is told..."),
			new Screenshot("screenies/shot060222c","The first tutorial island."),
			new Screenshot("screenies/shot060222d","In the north of the first tutorial Island.")
		)),
		array("Old isometric engine",array(
			new Screenshot("screenies/screenshot1","-"),
			new Screenshot("screenies/shot050228","-"),
			new Screenshot("screenies/shot050304","Trees are generated and rendered with a voxel/ray-casting technique."),
			new Screenshot("screenies/050511","Old 2.5d-engine. Graphics were generated using voxels and ray-casting. I wasted a lot of time on this. The Gui engine is already usable."),
		)),
		array("Very old plain 2d-engine",array(
			new Screenshot("screenies/screenshot_veryold2d","A very old screenshot. This is one of the very first attempts.")
		))
	);
	return $screenies;
}

function view()
{
	return menuSystem(includeTemplate("screenshots/view.thtml"),"screenshotBody");
}
?>
