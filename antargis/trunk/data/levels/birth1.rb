class Level1<Level
	def initialize
		puts "Level inited"
		start=StoryFlow.new("beginning")
		start.push("Godrin","Slowly you awake.")
		start.push("Godrin","You remember that you have been attacked, but ...")
		start.push("Godrin","Anything else is gone. You've lost your memory.")
		tellStory(start)
	end
	def eventTrigger(hero,t)
		case t.name
			when "nogo"
				if hero.getPlayer.class==AntHumanPlayer
					s=StoryFlow.new("noFurther")
					s.push(hero.getName,"I will not go any further!")
					tellStory(s)
					hero.newHLMoveJob(0,AGVector2.new(65,30),0)
				end
			when "storyFinished"
				if @story.name=="end"
					wonLevel
				end
		end
		return false # ignore
	end
	def eventOwnerChanged(ent)
		case ent.getName	
			when "Godrin"
				lostLevel
			when "Bantor"
				wonLevel
		end
	end
end