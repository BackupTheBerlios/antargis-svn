class Tutorial0<Level
	def initialize
		puts "Level inited"
		#exit
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