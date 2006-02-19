class Level1<Level
	def initialize
		puts "Level inited"
		#exit
		start=StoryFlow.new("beginning")
		start.push("Tutorial","At the beginning of each level your hero is centered.")
		start.push("Tutorial","You can select him by clicking on him.")
		start.push("Tutorial","After selecting the hero activate the recruit-button and then click on the tower.")
		tellStory(start)
	end
	def eventTrigger(hero,t)
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

end