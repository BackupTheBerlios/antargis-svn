<?
	include("includes.inc.php");
	ob_start();
	initSession();
	$menu=makeMenu();
		
	showPage(getPageID());
?>