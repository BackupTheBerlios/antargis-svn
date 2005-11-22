<?

function view()
{
	$b=h1("Download");
	
	$link=anchor("http://developer.berlios.de/svn/?group_id=3474","Berlios' SVN instructions");
	
	$b.=p("As this game is still in heavy development there are no packages or tarballs yet. But you can try the current state in svn.");
	$b.=p("For SVN-instruction please have a look at $link");
	
	$b.=p("You have to check out 'antargis' and 'antargisgui':");
	$b.=p("svn checkout svn://svn.berlios.de/antargis/antargisgui/trunk antargisgui","code");
	$b.=p("svn checkout svn://svn.berlios.de/antargis/antargis/trunk antargis","code");
	
	$b.=p("After that you have to install the antargis-gui library with the triple:");
	$b.=p("cd antargisgui","code");
	$b.=p("./configure && make","code");
	$b.=p("Get root and install by");
	$b.=p("su -c make install","code");
	$b.=p("The actual game, can be build with:");
	$b.=p("cd ../antargis","code");
	$b.=p("./configure && make","code");
	$b.=p("After that run the antargis.rb script in the antargis directory:");
	$b.=p("./antargis.rb","code");
	
	$b.=h3("I appreciate any helpful feedback, but please note, that this game is far from being stable or even playable.");
	
	return menuSystem($b);
}

?>