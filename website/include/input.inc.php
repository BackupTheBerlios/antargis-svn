<?

function getInput($a)
{
	if($_SESSION[$a]!="")
		return $_SESSION[$a];
	if($_POST[$a]!="")
		return $_POST[$a];
	else
		return $_GET[$a];
}

?>