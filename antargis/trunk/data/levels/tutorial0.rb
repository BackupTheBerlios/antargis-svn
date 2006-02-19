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
	def eventHLJobFinished(hero,job)
		puts "eventHLJobFinished(hero,job)"
		puts job.class,hero.getName
		#exit
		if job.class==AntHeroRecruitJob and hero.getName=="Godrin"
			start=StoryFlow.new("recruit")
			start.push("Tutorial","Ok, you have recruited some men.")
			start.push("Tutorial","If you want to recruit all men select the button with the three swords.")
			start.push("Tutorial","This increases your aggressiveness. After that repeat the recruiting.")
			start.push("Tutorial","When you're done with this go north and defeat your enemies.")
			tellStory(start)
		
		end
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