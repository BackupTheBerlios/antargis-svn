<?

function initSession()
{
	session_start();
	debug("initSession()");
}


function fromFormToSession($i,$s)
{
	$in=getInput($i);
	if($in!="")
		$_SESSION[$s]=$in;
	debug("fromFormToSession($i,$s):$_SESSION[$s]:$in");
}

function toSession($a,$b)
{
	$_SESSION[$a]=$b;
}

?>