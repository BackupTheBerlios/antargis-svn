<?
function zeige($x,$i=0)
{
	$ret="";
	foreach($x as $a=>$b)
	{
		for($j=0;$j<$i;$j++)
			$ret.="&nbsp;";
		$ret.="$a=>$b<br>\n";
		if(is_array($b))
			$ret.=zeige($b,$i+2);
	}
	return $ret;
}

function view()
{
	$test=zeige($GLOBALS);	

	return menuSystem($test);
}
?>