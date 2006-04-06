class Level1<Level
	def initialize
		@sheep=false
		@sack=false
		@defeat=false
		@takeFood=false
	end
	def eventLevelStarted
		if not @started
			start=StoryFlow.new("beginning")

			start.push("Tutorial - Food","On this island you will learn that your troops need food and how to get it.")
			start.push("Tutorial - Food","Your troops get always hungry. The current hunger of your hero can be seen in the brown bar at his portrait.")
			
			start.push("Tutorial - Food","You can view your troops state by selecting one and then inspecting the bars on the right.")
			start.push("Tutorial - Food","The more you do the faster they get hungry.")
			start.push("Tutorial - Food","So start now fetching food by slaughtering the sheep. (Right click)")
			tellStory(start)
			@started=true
		end
	end
	def eventHLJobFinished(hero,job)
		if hero.getName!="Godrin"
			return
		end
		if job.class==AntHeroFightAnimalJob
			if job.target.class==AntNewSheep and (not @sheep)
				@sheep=true
				start=StoryFlow.new("sheep")
				start.push("Tutorial","Now your troops have shared the food among them as you can see in their inventories.")
				start.push("Tutorial","Whenever they rest and are really hungry they will eat their food automatically. But you have to look that they have time to rest for this.")
				start.push("Tutorial","It seems like someone has left a bag a little down here. Take it up. (Right click)")
				tellStory(start)
			elsif job.target.class==AntSack
				@sack=true
				start=StoryFlow.new("sheep")
				start.push("Tutorial","Well done, now complete your task by conquering the farm and taking their food.")
				tellStory(start)
			end
		elsif job.class==AntHeroFightJob
			@defeat=true
			start=StoryFlow.new("sheep")
			start.push("Tutorial","Now take the food by selecting the farm and clicking on the 'take food' button.")
			tellStory(start)
		elsif job.class==AntHeroTakeJob
			@takeFood=true
		end
		if (@sheep and @defeat and @takeFood and @sack)
			wonLevel
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

end