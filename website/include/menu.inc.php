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
		return span($t,"menu_bold");
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
				$c.=" | ".tr(td("&nbsp;","menu_side").td(showMenu($a,true)));
			}
		}
		if($submenu)
			return table($c,"submenu");
		else
			return table($c,"menu");
	}
	
	function showMenu2($menu,$started=false)
	{
		$name=$menu[0];
		$title=$menu[1];
		$view=$menu[2];
		$ar=$menu[3];
		
		if($name==getInput("page"))
			$title=boldMenuItem($title);
		if($title!="")
			$c=anchor(page($name),$title);//,"menu_title");
		if(is_array($ar))
		{
			foreach($ar as $a)
			{
				if($c!="")
					$c.=" | ";
				$c.=showMenu2($a,true);
			}
		}
		if($started)
			return $c;
		else
			return $c;//table(tr($c),"full_width");
	}
	
	function menuSystem($body)
	{
		global $menu;
		
		$title=getTitle();
		$title=anchor("index.php?page=about",$title);
		
		$host='<table><tr><td valign="center" class="host">Kindly hosted by:</td><td><a href="http://developer.berlios.de" title="BerliOS Developer"> <img src="http://developer.berlios.de/bslogo.php?group_id=3474" width="124px" height="32px" border="0" alt="BerliOS Developer Logo"></a>&nbsp;</td></tr></table>';
		
		$menu=showMenu2($menu);
		
		$notice="(C) 2005 by the Battles of Antargis team";
		
		return div($title,"title").div($menu,"menu").div($body,"body").div($notice,"notice").div($host,"host");
	}

?>