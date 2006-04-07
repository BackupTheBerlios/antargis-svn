class Level1<Level
	def initialize
		@started=false
	end
	def eventLevelStarted
		if not @started
			start=StoryFlow.new("beginning")

			start.push("Tutorial - Inventing","On this island you will learn about inventing.")
			start.push("Tutorial - Inventing","You have to conquer the tower in the north. But it is strong for you.")
			start.push("Tutorial - Inventing","So you must increase your strength. To do so take over the workshop and press the invent-button when you have selected the workshop.")
			tellStory(start)
			@started=true
		end
	end
	def eventHLJobFinished(hero,job)
		if hero.getName!="Godrin"
			return
		end
		if job.class==AntHeroFightJob
			if job.target.class==AntWorkshop
				@defeat=true
				start=StoryFlow.new("sheep")
				start.push("Tutorial","Press the invent button. But beware that your men do rest enough. When you have enough weopons and armour you can take it all up by pressing the 'take weapon' button.")
				tellStory(start)
			else
				if hero.getPlayer==getMap.getPlayer
					wonLevel
				else
					lostLevel
				end
			end
		end
	end
	def eventTrigger(hero,t)
		case t.name
			when "storyFinished"
				if @story.name=="end"
					endLevel
				end
		end
		puts "TRIGGER"
		return false #ignore
	end
	def wonLevel
		return if @won 
		super
		@won=true
		start=StoryFlow.new("end")
		start.push("Tutorial","You finished this level.")
		tellStory(start)
	end
	def lostLevel
		return if @lost 
		super
		@lost=true
		start=StoryFlow.new("end")
		start.push("Tutorial","You lost!")
		tellStory(start)
	end

end