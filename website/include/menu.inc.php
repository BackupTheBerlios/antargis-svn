<?

	function menu($name,$title,$view,$ar="")
	{
		return array($name,$title,$view,$ar);
	}

	function thisPage()
	{
		return page(getInput("page"));
	}	
	
	function page($view="")
	{
		if(substr($view,0,4)=="http")
			return $view;
		if($view=="")
			$view=getInput("page");
		return "index.php?page=$view&dummy=".rand();
	}
	
	function boldMenuItem($t)
	{
		return p($t,"menu_bold");
	}
	
	function showMenu($menu,$submenu=false)
	{
		$name=$menu[0];
		$title=$menu[1];
		$view=$menu[2];
		$ar=$menu[3];
		
		if($name==getInput("page"))
			$title=boldMenuItem($title);
		$c=tr(td(anchor(page($name),$title),"menu_title","",2));
		
		if(is_array($ar))
		{
			foreach($ar as $a)
			{
				$c.=tr(td("&nbsp;","menu_side").td(showMenu($a,true)));
			}
		}
		if($submenu)
			return table($c,"submenu");
		else
			return table($c,"menu");
	}
	
	function menuSystem($body)
	{
		global $menu;
		
		$status=getStatus();
		
		if($status)
			$status=tr(td($status,"side_menu"));
			
		$title=getTitle();
		$title=anchor("index.php?page=about",$title);
		
		$host='Hosted by:<a href="http://developer.berlios.de" title="BerliOS Developer"> <img src="http://developer.berlios.de/bslogo.php?group_id=3474" width="124px" height="32px" border="0" alt="BerliOS Developer Logo"></a>';
		
		return body(table(tr(td($title,"title","",2)).tr(td(showMenu($menu),"side_menu").td($body)).tr(td($host,"title","",2)),"full_width"));
	}

?>