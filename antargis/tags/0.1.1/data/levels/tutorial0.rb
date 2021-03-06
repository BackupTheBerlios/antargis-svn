class Level1<Level
	def initialize
		puts "Level inited"
	end
	def eventLevelStarted
		if not @started
			start=StoryFlow.new("beginning")

			start.push("Tutorial - Exploring your world.","Welcome to the Battles Of Antargis Tutorial. At first you will be shown how to move around in your world.")
			start.push("Tutorial - Exploring your world.","For fast moving your view you can click on the 'Mini-Map' on the lower right (which will be shown as soon as this tutorial is over).")
			start.push("Tutorial - Exploring your world.","If you want to scroll your view you can push and hold the right mouse-button and move it around. For zooming you can use your mouse-scrollers.")
			start.push("Tutorial - Exploring your world.","When you are in Fullscreen-mode you scroll by moving the cursor to screen's borders. Push ALT+RETURN to toggle fullscreen mode.")

			start.push("Tutorial","Now lets start with the actual game. At the beginning of each level your hero is centered and selected. Unlike other games you can only control your heroes.")
			start.push("Tutorial","You can move him by left-clicking on the terrain. Please wait with testing this until this small instructions are finished.")
			start.push("Tutorial","Now recruit some troops. Select the tower by left-clicking on it and then push the recruit-button on the right.")
			tellStory(start)
			@started=true
		end
	end
	def eventHLJobFinished(hero,job)
		if job.class==AntHeroRecruitJob and hero.getName=="Godrin" and @recruit==nil
			@recruit=true
			start=StoryFlow.new("recruit")
			start.push("Tutorial","Ok, you have recruited some men.")
			start.push("Tutorial","If you want to recruit all men select the button with the three swords. This increases your aggressiveness. After that repeat the recruiting.")
			start.push("Tutorial","When you're done with this go north and defeat your enemies. You can fight an enemy by right clicking on him.")
			tellStory(start)
		end
	end
	def eventDismissed(hero)
		if @recruit and @won
			start=StoryFlow.new("end")
			start.push("Tutorial","You have passed the first tutorial level.")
			tellStory(start)
		end
	end
	def eventTrigger(hero,t)
		case t.name
			when "storyFinished"
				if @story.name=="end"
					wonLevel
					endLevel
				end
		end
		puts "TRIGGER"
		return false #ignore
	end
	def eventOwnerChanged(ent)
		case ent.getName	
			when "Godrin"
				lostLevel
			when "Bantor"
				wonLevel
		end
	end
	def wonLevel
		super
		@won=true
		start=StoryFlow.new("recruit")
		start.push("Tutorial","So you have defeated Bantor. Now go back to your Keep und dismiss some of your men.")
		tellStory(start)
	end

end