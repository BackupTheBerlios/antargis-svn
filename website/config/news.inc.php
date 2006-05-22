<?
function news($date,$title,$body="")
{
	//return h3($date." - ".$title).p($body);

	return "<div class='newstitle'>$title</div><div class='newsdate'>$date</div>".p($body);

}

function getNews($toponly=false)
{
	$b="";

	$b.=news("2006-05-22","IRC Meeting on 06-05-28","For further information go here: <a href='http://antargis.berlios.de/phpBB2/viewtopic.php?p=233#233'>News-Forum</a>");

	$b.=news("2006-05-19","Release of 0.1.2","This is a major speed improvement release. The game made a big jump and is sometimes doubling its performance. The second improvement is that the first level of our campaign is finished. For a complete changelog go to our wiki: <a href='http://antargis.berlios.de/wiki/index.php/Release_Plan#Version_0.1.2'>Release Plan in wiki</a>.");


	$b.=news("2006-05-16","0.1.2 about to be released","Version 0.1.2 is nearly finished and it would be really cool, if some you could give it short test for showstoppers. A changelog is here: <a href='http://antargis.berlios.de/wiki/index.php/Release_Plan#Version_0.1.2'>Release Plan in wiki</a>.<br>The download link: <a href='http://developer.berlios.de/project/showfiles.php?group_id=3474'>Downloads</a>. It can be that I'll do some more work on the second campaign level, but that's not that important.");

	$b.=news("2006-04-23","Forum back online again","I'm very happy to anounce, that the forums and the wiki are back online again.");
	
	$b.=news("2006-04-20","Forum and Wiki down","I'm sorry to tell you, that since yesterday the forums and the wiki are down. It's a problem with a database server at our provider Berlios. We have submitted an error message and hope that the problem will be solved soon.<br>In the meanwhile you can contact us on our mailing-list. Have a look the <a href='index.php?page=contact'>contact page</a>.");

	$b.=news("2006-04-14","Version 0.1.1 out","Today the version 0.1.1 is released, which contains mostly bugfixes. The most outstanding new feature is inventing. Now you are able to build swords and shields.<br><a href='http://developer.berlios.de/project/showfiles.php?group_id=3474'>Download here</a>");
	if(!$toponly)
{


	$b.=news("2006-04-09","Website update &amp; current state","The website was updated. Apart from this several new things were added in SVN:
<ul>
	<li>many more inventory items</li>
	<li>inventing is done</li>
	<li>sword and shield is used when fighting</li>
	<li>two more tutorial-islands</li>
	<li>some bugs concerning AMD64 and FreeBSD were solved</li>
	<li>a segfault was fixed</li>
	<li>new job-display</li>

</ul>");

	$b.=news("2006-04-03","Help anouncement","This weekend I filed a help wanted message at the Linux Game Tome, but this little impact yet. Maybe it's to come after a first appearance on the front page, as I have submitted it as a new game.");
	$b.=news("2006-03-23","Forum","Come to the forum and discuss the game's idea, screenshots and whatever. I'll very soon anounce the first tech-demo, which will hopefully gain some interest and maybe gather some developers and artits to join!");


	$b.=news("2005-12-08","Music","I've made a first attempt to create some music: <a href='http://antargis.berlios.de/wiki/index.php/Music'>music in wiki</a>");
	$b.=news("2005-12-07","Wiki !","A (development-)wiki has been added. <a href='wiki/'>go to wiki</a>");
	
	$f="<ul>
		<li>complete rewrite of rendering and switch to 3d !!</li>
		<li>some of the game logic is already there. You can already fight and conquer castles.</li>
		<li>men run around fetch fruit, stone and wood and produce tools in the workshop</li>
		<li>a very rudimentary AI is implemented, but not really working yet.</li>
	</ul>";
	$b.=news("2005-11-22","Great progress","News writing these days is somewhat useless as I've done no promotion yet. But behind the scene there is pretty much new:$f");
	$b.=news("2005-04-12","Homepage update","Martin Kamphausen provided some nice logo. AntargisGUI-lib is proceeding - XML-support for layouting is included now.");
	$b.=news("2005-04-05","Homepage setup","Welcome to to first version of this website!<br>There is nothing much here at the moment, but it is soon to come.");
}
	return $b;
}


?>
