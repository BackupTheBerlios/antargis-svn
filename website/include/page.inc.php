<?

function getPageID()
{
	return getInput("page");
}

function pageNotFound($page)
{
	return html(head().body("Page not found:$page"));
}

function getPath($page,$menu)
{
	if($menu[0]==$page)
		return $menu[2];
	else if(is_array($menu[3]))
		{
			foreach($menu[3] as $m)
			{
				$r=getPath($page,$m);
				if($r!="")
					return $r;
			}
		}
	return "";
}

function getPage($page)
{
	global $menu;
	$p=getPath($page,$menu);
	if($p=="")
		return pageNotFound($page);
	include($p);
	return html(head().view());
}

function getPageDB($page)
{
	global $db;
	$q="select path from pages where name='$page'";
	$r=$db->query($q);
	if($db->num_rows($r)==0)
		return pageNotFound($page);
	$row=$db->result($r);
	include($row[path]);
	return html(head().view());
}

function showPage($page)
{
	$p=getPage($page);
	echo $p;
}

?>