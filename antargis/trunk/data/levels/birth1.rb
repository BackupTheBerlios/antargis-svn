class Level1<Level
	def initialize
		puts "Level inited"
		tellStory("Godrin","Slowly you awake.")
	end
	def eventTrigger(hero,t)
		case t.name
			when "nogo"
				if hero.getPlayer.class==AntHumanPlayer
					tellStory(hero.getName,"I will not go any further!")
					hero.newHLMoveJob(0,AGVector2.new(65,30),0)
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