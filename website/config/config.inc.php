<?
$dbuser="sh";
$dbpass="bubi";
$dbname="phpproject";
$dbhost="localhost";

$DEBUGGING=false;

function getTitle()
{
	return image("pics/antargisLogo.png");//div("PHPproject","title");
}

function showInfo()
{
	return "<small>(C) 2004 by ".anchor("mailto:david@wilmskamp.dyndns.org","David Kamphausen")."</small>";
}

function getStatus()
{
	global $DEBUGGING;
	if($DEBUGGING)
	{
		debug("POST:");
		foreach($_POST as $a => $b)
		{
			debug("POST:$a=>$b");
		}
	}
	
	
	$project=getInput("Projekt");
	if($project!="")
	{
		return table(tr(td("Gewähltes Projekt:")).tr(td(getProjectName($project),"projekt_status")),"status");
	}
	return "";
}

?>