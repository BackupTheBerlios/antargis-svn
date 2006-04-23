<?

function view()
{
	$b=h1("Download");

	$version="0.1";

	$b.=p("Get the current release here:");
	$l=anchor("http://developer.berlios.de/project/showfiles.php?group_id=3474","Download a release at berlios.de");
        $b.=p($l,"link");
	$b.=p("Further instructions can be found below or in the <a href='http://svn.berlios.de/svnroot/repos/antargis/antargis/trunk/README'>README</a> and <a href='http://svn.berlios.de/svnroot/repos/antargis/antargis/trunk/INSTALL'>INSTALL</a> file within the package.");



	$b.=h3("Compiling");	
	$b.=p("After you have downloaded the source-distribution you do the followin:");
	$b.=p("tar xfz antargis-0.1.tar.gz","code");
	$b.=p("cd antargis-0.1","code");
	$b.=p("rake","code");
	$b.=p("After that run the run.rb script:");
	$b.=p("./run.rb","code");

	$b.=h3("Problems");
	$b.=p("If you come accross problems please go to our User FAQ page at <a href='http://antargis.berlios.de/wiki/index.php/User_FAQ'>our Wiki</a>");


	$b.=h3("Get Antargis from SVN");	
	$svn=anchor("http://developer.berlios.de/svn/?group_id=3474","Berlios' SVN instructions");
	$b.=p("As this game is still in heavy development there are no packages or tarballs yet. But you can try the current state in svn.");
	$b.=p("For SVN-instruction please have a look at $svn");
	
	$b.=p("svn checkout svn://svn.berlios.de/antargis/antargis/trunk antargis","code");

	$b.=p("If you want to check antargisgui BoA's gui library, then check out this way:");
	$b.=p("svn checkout svn://svn.berlios.de/antargis/antargis/trunk/gui antargisgui","code");


	$b.=h3("I appreciate any helpful feedback, but please note, that this game is far from being stable or even playable.");

	
	return menuSystem($b);
}

?>
