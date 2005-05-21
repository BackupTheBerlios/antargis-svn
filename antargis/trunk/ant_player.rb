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
	def jobFinished
	end
	def saveXML(n)
		n.set("name",@name)
		@heroes.each{|hero|
			c=n.newChild("hero")
			c.set("name",hero.getName)
		}
	end
	def loadXML(n)
		@name=n.get("name")
		children=n.get_children
		children.each{|c|
			@heronames.push(c.get("name"))
		}
	end
	def move(time)
		if @heronames.length then
			@heronames.each{|n|
				ent=getMap.getByName(n)
				if ent
					ent.setPlayer(self)
					@heroes.push(ent)
				else
					puts "HERO NOT FOUND!"
				end
			}
			@heronames.clear
		end
	end
	
	def assignJob(hero)
		puts "ANTPLAER::assignJob"
		# simply attack
		target=getMap.getNext(hero,"hero")
		hero.newHLFightJob(target)
	end
end

class AntHumanPlayer<AntPlayer
	def jobFinished(who,what)
		puts who+" is ready with is job:"+what
	end
	def xmlName
		return "humanPlayer"
	end
end

class AntComputerPlayer<AntPlayer
	def initialize()
		super
	end
	def jobFinished
	end
	def xmlName
		return "computerPlayer"
	end
end