#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_player.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

#!/usr/bin/ruby

class AntPlayer
	def initialize(name)
		@heroes=[]
		@heronames=[]
		@name=name
	end
	def eventJobFinished
	end
	def saveXML(n)
		n.set("name",@name)
		@heroes.each{|hero|
			c=n.newChild("hero")
			c.set("name",hero.getName)
		}
	end
	def loadXML(n)
		puts "a"
		@name=n.get("name")
		puts "b"
		children=n.get_children("hero")
		puts "c"
		children.each{|c|
			@heronames.push(c.get("name"))
		}
	end
	def move(time)
		if @heronames.length then
			@heronames.each{|n|
				ent=getMap.getByName(n)
				if ent
					puts ent
					puts ent.getName
					ent.setPlayer(self)
					@heroes.push(ent)
				else
					puts "HERO NOT FOUND!"
				end
			}
			@heronames.clear
		end
	end
	def getName
		@name
	end
	
	def assignJob(hero)
		hero.newHLRestJob(20)
	end
end

class AntHumanPlayer<AntPlayer
	def eventJobFinished(who,what)
		puts who.to_s+" is ready with is job:"+what.to_s
	end
	def xmlName
		return "humanPlayer"
	end
	def assignJob(hero)
		hero.newHLRestJob(20)
	end
end

class AntComputerPlayer<AntPlayer
#	def eventJobFinished
#	end
	def xmlName
		return "computerPlayer"
	end
	def eventJobFinished(who,what)
		puts who.to_s+" is ready with is job:"+what.to_s
	end
	def assignJob(hero)
		puts "ANTPLAER::assignJob"
		# simply attack
		target=getMap.getNext(hero,"hero")
		
		if target and target.getPlayer!=self
			hero.newHLFightJob(target)
		else
			house=getMap.getNext(hero,"house")
			housePos=house.getPos2D
			heroPos=hero.getPos2D
			diff=housePos-heroPos
			if diff.length2>150
				hero.newHLMoveJob(0,house.getPos2D,100)
			else
				hero.newHLRestJob(20)
			end
		end
	end
end