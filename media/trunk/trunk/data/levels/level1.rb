class Level1<Level
	def initialize
		puts "Level inited"
		#tellStory("title","Level started")
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