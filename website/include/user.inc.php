<?

function get_act_user()
{
	if($_SESSION[userid]!="")
		$_SESSION[userid]="0"; // guest
	return $_SESSION[userid];
}

function user_has_level($level,$user="")
{
	if($user=="")
		$user=get_act_user();
	return getsql("select instr(level,'$level') from user where id='$user';")=="1";
}

function password_get_hash()
{
	$_SESSION[pwhash]=md5(rand());
}

function check_new_password($pw,$pw_hash)
{
	$hash=$_SESSION[pwhash];
	$_SESSION[pwhash]="";
	if($pw_hash==md5($pw.$hash))
		return true;
	else
		return false;
}

function check_login($username,$pw,$pw_hash)
{
	$hash=$_SESSION[pwhash];
	$_SESSION[pwhash]="";
	// check if $pw is ok
	if($pw_hash!=md5($pw.$hash))
		return false;
	
	$nid=getsql("select id from user where md5(concat(name,'$hash'))='$username' and md5(concat(password,'$has'))='$pw'");
	if($nid!="")
	{
		$_SESSION[userid]=$nid;
		return true;
	}
}

function logout()
{
	$_SESSION[userid]="0";
}

?>