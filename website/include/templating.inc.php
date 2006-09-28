<?
function startTemplateEngine()
{
    ob_start();
}


function includeTemplate($file,$ar=array())
{ 
	$buffer = ob_get_contents();
	extract($ar);
	include $file; 
	$output = substr(ob_get_contents(),strlen($buffer)); 
	ob_end_clean(); 
	
	ob_start(); 
	echo $buffer; 
	
	return $output; 
} 

?>
