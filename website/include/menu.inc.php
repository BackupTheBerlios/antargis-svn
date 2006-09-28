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
		if($t!="")
			return span($t,"menu_bold");
		return $t;
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
	
	function menuSystem($body,$bodyclass="body")
	{
		global $menu;
		
		$title=includeTemplate("views/title.thtml");
		
		$menu=showMenu2($menu);
		
		return div($title,"title").div($menu,"menu").div($body,$bodyclass).includeTemplate("views/bottom.thtml");
	}

?>