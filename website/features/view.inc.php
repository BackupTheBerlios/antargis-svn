<?

function view()
{
	$b=h1("Features");
	
	$b.=h3("Visuals");
	$b.="<ul>
		<li>animated milkshape models (MilkShape ASCII-importer)</li>
		<li>static models (Wavefront obj-importer)</li>
		<li>shadowmap based shadows (PSMs work in progress)</li> 
	</ul>";

	$b.=h3("Game");
	$b.="<ul>
		<li>the whole gamelogic is done in ruby, so it should be fairly easy to modify this game.</li>
		<li>the whole layout is done in xml</li>
		<li>basic dialogs and intro-screens implemented, but nice graphics still missing</li>
		<li>loading/saving</li>
	</ul>";
	
	$b.=h3("Editor");
	$b.="<ul>
		<li>edit heightmap</li>
		<li>place entities on map</li>
	</ul>";
	
	$b.=h1("Planned features");
	
	$b.=h3("Visuals");
	$b.="<ul>
		<li>complex lighting with day and night cycles</li>
		<li>seasons - snow, rain, wind blowing leaves</li>
		<li>mud</li>
	</ul>";
	$b.=h3("Game");
	$b.="<ul>
		<li>several campaigns with straight storyline</li>
		<li>networking with chat</li>
	</ul>";
	
	$b.=h3("Editor");
	$b.="<ul>
		<li>define triggers</li>
		<li>place splats/decals on map</li>
	</ul>";
	return menuSystem($b);
}

?>