class Level1<AntLevelScript
	def eventLevelStarted
		puts "Level inited"
	end
	def eventTrigger(hero,t)
		puts "TRIGGER"
		return false #ignore
	end
	def eventOwnerChanged(ent)
		case ent.getName	
			when "Rowen"
				lostLevel
			when "Bantor"
				wonLevel
		end
	end

end