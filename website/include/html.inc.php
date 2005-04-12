<?

function center($c)
{
	return "<center>$c</center>";
}

function indent($data)
{
	return $data;
	$x=str_replace("\n","\n  ",$data);
	
	return rtrim($x)."\n";
}

function element($name,$a,$c="",$style="",$z="")
{
	if($style!="")
		$style=" style='$style'";
	if($c!="")
		$c=" class='$c'";

	if($a!="")
		return "<$name$c$z$style>\n".indent($a)."</$name>\n";
	else
		return "<$name$c$z$style></$name>\n";
}

// normal HTML
function html($a,$c="")
{
	return element("html",$a,$c);
}
function body($a,$c="")
{
	return element("body",$a,$c);
}

function head()
{
	return element("head","<link rel=stylesheet type='text/css' href='config/style.css'><title>Battles of Antargis</title>");
}

function image($a,$c="")
{
	return element("img","",$c,""," src='$a'");
}

// TABLES
function table($a,$c="",$s="",$add="")
{
	$add=" cellpadding='0' cellspacing='0'";
	return element("table",$a,$c,$s,$add);
}
function tr($a,$c="")
{
	return element("tr",$a,$c);
}
function td($a,$c="",$style="",$cs="",$rs="",$add="")
{
	if($cs!="")
		$cs=" colspan='$cs'";
	if($rs!="")
		$rs=" rowspan='$rs'";
	return element("td",$a,$c,$style,"$cs$rs$add");
}
function td2($a,$c="",$style="")
{
	return element("td",$a,$c,$style," colspan='2'");
}
function td_2($a,$c="",$style="")
{
	return element("td",$a,$c,$style," rowspan='2'");
}
function td22($a,$c="",$style="")
{
	return element("td",$a,$c,$style," rowspan='2' colspan='2'");
}
function td3($a,$c="",$style="")
{
	return element("td",$a,$c,$style," colspan='3'");
}

// window
function window($title,$content,$c="window",$shadow=false)
{
	$l=tr(td("","window_line"));
	$r=table(tr(td($title,"window_title")).$l.$l.tr(td($content,"window_body")),$c);
	
	if($shadow)
	{
		$r=table(tr(td22($r).td(image("pics/shadow1.png"),"","height:20px;")).
						 tr(td("","","background-image:url(pics/shadow2.png);background-repeat:repeat-y;")).
						 tr(td(image("pics/shadow5.png"),"","width:32px;").td("&nbsp;","","background-image:url(pics/shadow4.png);background-repeat:repeat-x;").td(image("pics/shadow3.png"))));
	}
	
	return $r;
}

function insetWindow($title,$content,$c="window_inset")
{
	$l=tr(td("","window_line"));
	return table(tr(td($title,"window_inset_title")).$l.$l.tr(td($content,"window_body")),$c);
}

function border($a)
{
	return div($a,"border");
}

// divs
function div($a,$c="",$s="")
{
	return element("div",$a,$c,$s);
}
function span($a,$c="",$s="")
{
	return element("span",$a,$c,$s);
}

function p($a,$c="",$s="")
{
	return element("p",$a,$c,$s);
}

// anchor

function anchor($url,$a,$c="")
{
	return element("a",$a,$c,""," href='$url'");
}

function button($url,$a,$c="button")
{
	return table(tr(td(anchor($url,$a))),$c);
}

function buttonBorder($a,$c="button")
{
	return table(tr(td($a)),$c);
}

// forms
function form($name,$a="",$action="",$z="")
{
	if($action!="")
		$action=" action='$action'";
	$a.=inputHidden("dummy",rand());
	return element("form",$a,"",""," method='post' id='$name' name='$name'$action");
}

function autoform($name,$a="",$z="")
{
	return form($name,$a,"index.php?page=".getInput("page"),$z);
}

function option($name,$value,$add="")
{
	return "<option value='$value'$add>$name</option>\n";
}

function select($name,$ops,$sel="",$maxn=10,$autosubmit=false)
{
	$c="";
	foreach($ops as $o=>$p)
	{
		if($sel==$o)
			$c.=option($p,$o," selected");
		else
			$c.=option($p,$o);
	}
	$size=min($maxn,sizeof($ops));
	
	$add="";
	if($autosubmit)
		$add=" onchange='submit();'";
	
	return element("select",$c,"",""," size='$size' id='$name' name='$name'$add");
}

function inputText($name,$value="",$c="")
{
	return element("input",$c,"",""," value='$value' type='text' id='$name' name='$name'");
}

function inputLongText($name,$value="",$c="")
{
	return "<textarea class='$c' name='$name' id='$id' cols='80' rows='8'>$value</textarea>\n";
}

function inputHidden($name,$c="")
{
	return element("input","","",""," type='hidden' id='$name' name='$name' value='$c'");
}

function submit($a="Abschicken",$c="")
{
	return element("input","",$c,""," type='submit' value='$a' name='$a'");
}

function debug($a)
{
	global $DEBUGGING;
	if($DEBUGGING)
		echo p("DEBUG:".$a,"debug");
}

/* headlines */

function h1($a,$c="",$s="")
{
	return element("h1",$a,$c,$s);
}
function h2($a,$c="",$s="")
{
	return element("h2",$a,$c,$s);
}
function h3($a,$c="",$s="")
{
	return element("h3",$a,$c,$s);
}

/* javascripting */
function setFormElementInline($name,$value,$form="")
{
	return "javascript:document.getElementById(\"$name\").value=\"$value\";document.getElementById(\"$form\").submit();";
}

/* formatting */
function formatComment($text)
{
	$otext=$text;
	if(strpos($text,"HTML"))
	{
		return str_replace("HTML","",$text);
	}
	else
	{
		$wiki="index.php?page=wiki";
	
		
		//headlines
		$pat="/^!([^!\n]*)$/m";
		$rep="<h3 class='wiki'>$1</h3>";
		$text=preg_replace($pat,$rep,$text);
		//headlines
		
		$pat="/^!!([^!\n]*)$/m";
		$rep="<h2 class='wiki'>$1</h2>";
		$text=preg_replace($pat,$rep,$text);
		
		$pat="/^!!!([^!\n]*)$/m";
		$rep="<h1 class='wiki'>$1</h1>";
		$text=preg_replace($pat,$rep,$text);
		
		
		// check links
		$pat="/(http:\/\/[öäüÖÄÜß\w\/\.]*)/";
		$rep="<a href='$1'>$1</a>";
		$text=preg_replace($pat,$rep,$text);
		
		// [.*] to phpwiki
		$pat="/\[([\wöäüÖÄÜß]*)\]/";
		$rep="<a href='$wiki&WikiName=$1'>$1</a>";
		$text=preg_replace($pat,$rep,$text);
		
		
		// WikiWords to phpwiki
		$pat="/[^\w\/=&>]([A-ZÖÄÜ][öäüÖÄÜß\w]+[A-ZÖÄÜ]\w+)/";
		$rep=" <a href='$wiki&WikiName=$1'>$1</a>";
	
		$text=preg_replace($pat,$rep," $text");
		
		//echo "text:$text<br>\n";
		// double-nl to br
		$text=str_replace("\n","<br>\n",$text);
		
		$text=preg_replace("/(<\/h[123]>)<br>\n/","$1\n",$text);
		
		$pat="/\{(\d*)\}/";
		$rep="<a href='index.php?page=Ticket&TicketID=$1'>[Ticket# $1]</a>";
		$text=preg_replace($pat,$rep,$text);
		
		return $text;
	}

}

?>