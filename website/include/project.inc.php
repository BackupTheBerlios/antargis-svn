<?

function getComments($hideDel=true)
{
	global $db;
	$id=getInput("Part");
	if($hideDel)
		$r=$db->query("select Comment,Date,UserID,Status from PartComment where PartID=$id and Status=0 order by Date desc;");
	else
		$r=$db->query("select Comment,Date,UserID,Status from PartComment where PartID=$id order by Date desc;");
	$res=array();
	for($i=0;$i<$db->num_rows($r);$i++)
	{
		$row=$db->result($r);
		$res[$i]=$row;
	}
	return $res;
}

function getProjectName($id)
{
	return getPartName($id);
}

function getPartName($id)
{
	if($id=="")
		return "";
	return getsql("select name from Part where id=$id;","name");
}

function getProjects()
{
	global $db;
	$q="select id,name from Part where parentID=0";
	
	$r=$db->query($q);
	$a=array();
	for($i=0;$i<$db->num_rows($r);$i++)
	{
		$row=$db->result($r);
		$a[$row[id]]=$row[name];
	}
	
	return $a;
}

function getParts($part="")
{
	global $db;
	
	$project=getInput("Part");
	
	
	if($part=="")
		$part=getInput("Part");
		
	$q="select id,name from Part where parentID=".$part;
	$r=$db->query($q);
	$a=array();
	for($i=0;$i<$db->num_rows($r);$i++)
	{
		$row=$db->result($r);
		$a[$row[id]]=$row[name];
	}
	
	return $a;
}

function getParentPart($partid)
{
	return getsql("select parentID from Part where id=$partid","parentID");
}

function renamePart($pname)
{
	global $db;
	
	$db->query("update Part set name='$pname' where id=".getInput("Part"));
}

function newProject($name)
{
	global $db;
	$db->query("insert into Part (name) values ('$name');");
}

function newPart($name)
{
	global $db;
	$id=getsql("select id from Part where id=".getInput("Part"),"id");

	$db->query("insert into Part (name,parentID) values ('$name','$id');");
}

function insertNewComment($nc)
{
	global $db;
	$id=getInput("Part");
	$db->query("insert into PartComment (PartID,Comment,Date) values ($id,'$nc',now())");
}

function getConnections($partid,$cid)
{
	global $db;
	return getsqlarray("select Part.id,name from Part,PartConnect where Part1ID=$partid and ConnectID=$cid and Part.id=Part2ID");
}

?>