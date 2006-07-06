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
	attr_accessor :name
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
				if ent
					ent.setPlayer(self)
					@heroes.push(ent)
				else
					puts "HERO NOT FOUND: '#{n}'"
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
	def xmlName
		return "computerPlayer"
	end
	def eventJobFinished(who,what)
		puts who.to_s+" is ready with is job:"+what.to_s
	end
	def assignJob(hero)
		hero.newHLRestJob(20)
	end
	
	# simple trigger
	def trigger(ent,tr)
		if tr.player!=@name
			return
		end
		if ent.getPlayer!=self and tr.name=="attack"
			@heroes.each{|h|
				if h.getJob.class==AntHeroRestJob
					puts "ASSIGNING!"
					h.newHLFightJob(ent)
				end
			}
		end
	end
	def attackNextHero(hero)
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
	def getNextHome(hero)
		getMap.getNext(hero,"house")
	end
	def goHome(hero,home)
		housePos=house.getPos2D
		heroPos=hero.getPos2D
		diff=housePos-heroPos
		if diff.length2>4
			hero.newHLMoveJob(0,house.getPos2D,2)
		end
	end
	def attack(hero,target)
		if hero and target and target.getPlayer!=self
			hero.newHLFightJob(target)
		end
	end
	
	def attackChecked(hero,target)
		if [AntHeroRestJob,NilClass].member?(hero.getJob.class)
			attack(hero,target)
		end
	end
	def getNextEnemy(hero)
		getNextEnemyX(hero,"hero")
	end
	def getNextEnemyHouse(hero)
		getNextEnemyX(hero,"house")
	end
	private
	def getNextEnemyX(hero,name)
		ts=getMap.getEntities(name)
		
		target=nil
		dist=10000
		ts.each{|tp|
			t=tp.get
			d=(t.getPos2D-hero.getPos2D).length
			if d<dist and t.getPlayer!=self
				dist=d
				target=t
			end
		}
		return target
	end
end

class AntLazyPlayer<AntComputerPlayer
end

class AntConqueringPlayer<AntComputerPlayer
	def initialize(name)
		super
		@mode=:rest
	end
	def assignJob(hero)
		case @mode
			when :rest
				hero.newHLRestJob(50)
				@mode=:fight
			when :fight
				attackChecked(hero,getNextEnemyHouse(hero))
				@mode=:rest
		end
	end
end
