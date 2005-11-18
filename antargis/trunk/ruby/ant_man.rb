#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_man.rb
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

# try to implement alle entites in ruby
# WARNING: DON'T MEMBER_VARIABLES AS IT SEEMS TO CRASH RUBY SOMEHOW
# could be that it has something to do with Init_Stack ???


class AntNewAngel<AntMyEntity
	def initialize
		super(AGVector2.new(0,0))
		setType("angel")
		@age=0
	end
	def move(time)
		p=getPos3D
		p.y+=time*20
		setPos3D(p)
		@age+=time
		if @age>10 then # 10 seconds old
			# discard
			getMap.removeEntity(self)
			getMap.endChange
		end
	end
	def getTexture
		return "angel.png"
	end
end

class AntNewMan<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		setType("man")
		@signed=false
		@dead=false
		@fighting=false
		@bossName=""
		puts "NEWMAN"
		setMesh(Mesh.new(getMeshData("data/models/man.ant",0.7),AGVector4.new(0,0,0,0),0))
		@mode="wait"
	end
	def getTexture
		if @dead
			return "grave.png"
		elsif @fighting
			return "man1_sword1.png"
		else
			return "man"+mDirNum.to_s+".png"
		end
	end
	
	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end
	
	##########################
	# EVENTS
	##########################
	def eventNoJob
		eventJobFinished
	end
	
	def eventDefeated
		if @boss then
			@boss.eventManDefeated(self)
		end
	end
	
	def eventDie
		simDeath
		eventDefeated
		newRestJob(20)
	end
	
	# Job was finished
	# 1) if no energy or dead: simDeath
	def eventJobFinished
		super
		if getEnergy==0 or @dead then
			simDeath
			return
		end
		
		setFighting(false)
		
		if @bossName=="" then
		
			house=getMap.getNext(self,"house")
			if house
				houseName=house.getName
				if houseName=="" then
					puts "ERROR House has no name!"
					exit
				end
				@bossName=houseName
				newRestJob(rand()*2)
				house=getMap.getRuby(house)
				house.signUp(self)
				@signed=true
			end
		else
			boss=getMap.getByName(@bossName)
			if not boss
				@bossName=""
				return
			end
			@boss=boss
			if not @signed then
				puts @bossName
				puts boss
				boss.signUp(self)
				@signed=true
			end
			boss.assignJob(self)
		end
	end	
	
	##########################
	# setBoss
	##########################
	def setNoBoss()
		@bossName=""
	end
	
	def setBoss(hero)
		@bossName=hero.getName
		hero.signUp(self)
	end

	
	##########################
	# private
	##########################	
	def sendAngel
		e=AntNewAngel.new
		e.setPos(getPos2D)
		getMap.insertEntity(e)
		getMap.endChange
	end

	# simulate death:
	#   - transfer appearance to gravestone
	#   - start angel
	def simDeath
		if @dead
			if @boss
				@boss.removeMan(self)
			end
			getMap.removeEntity(self)
			getMap.endChange
			return 
		else
			@dead=true
			updateSurface
			sendAngel
		end
	end
	
	def setFighting(v)
		@fighting=v
	end

	def xmlName
		return "antNewMan"
	end
	
	def saveXML(node)
		super(node)
		node.set("bossName",@bossName)
	end
	def loadXML(node)
		super(node)
		@bossName=node.get("bossName")
	end
end

