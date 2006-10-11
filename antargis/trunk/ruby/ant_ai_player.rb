class AntAIPlayer
	def initialize(ai)
		@heroes=[]
		@heronames=[]
		@ai=ai
		@name=@ai.interface.getPlayerName
	end
	def saveXML(n)
		n.set("name",@name)
		@heroes.each{|hero|
			c=n.addChild("hero")
			c.set("name",hero.getName)
		}
	end
	def loadXML(n)
		@name=n.get("name")
		children=n.getChildren("hero")
		children.each{|c|
			@heronames.push(c.get("name"))
		}
	end

	def move(time)
		if @heronames.length then
			@heronames.each{|n|
				ent=getMap.getByName(n)
				if ent.is_a?(AntBoss)
					ent.setPlayer(self)
					@heroes.push(ent)
				else
					puts "HERO NOT FOUND: '#{n}'"
				end
			}
			@heronames.clear
		end
		@ai.eventFrame(time)
	end
	def getName
		@name
	end
	
	def assignJob(hero)
		hero.newHLRestJob(20)
	end
	def trigger(ent,tr)
	end
	def remove(hero)
		@heroes.delete(hero)
	end
	def add(hero)
		@heroes.push(hero)
		@heroes.uniq!
	end
	def getHeroes
		@heroes.uniq!
		@heroes.clone
	end


	def eventJobFinished(hero,lastJob)
		@ai.eventHeroWithoutJob(hero.uid)
	end

end