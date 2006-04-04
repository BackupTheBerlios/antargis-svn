#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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

class AntNewAngel<AntMyEntity

	def initialize
		super(AGVector2.new(0,0))
		setProvide("angel",true)
		@age=0
	end
	def move(time)
		p=getPos3D
		p.y+=time*20
		setPos(p)
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


module AntManBase
	def sitDown
		puts "sitDown"
		newRestJob(0.4)
		setMeshState("sitdown")
	end

	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end
	def eventGotFight(other)
		return
		puts "EVENTGOTFIGHT"
		if shouldFight and (not isFighting)
			#newFightJob(0,other)
		end
	end
	def eventDefeated
		if is_a?(AntBoss)
			eventManDefeated(self)
		elsif @boss then
			@boss.eventManDefeated(self)
			#@boss.removeMan(self)
			#@boss=nil
			#@bossName=""
		end
	end
	def eventNoJob
		if @meshState=="dead"
			getMap.removeEntity(self)
		end
		super
	end

	def eventDie
		puts "eventDie "+self.to_s
		simDeath
		#eventDefeated
		newRestJob(20)
		setMeshState("dead")
		if @boss
			@boss.removeMan(self)
		end
		if not self.resource.empty
			sack=AntSack.new(getPos2D+AGVector2.new(0.3,-0.3))
			getMap.insertEntity(sack)
			sack.resource.takeAll(self.resource)
		end
	end


	# simulate death:
	#   - transfer appearance to gravestone
	#   - start angel
	def simDeath
		if @dead
			return
		else
			@dead=true
			#setMesh(Mesh.new(getMeshData("data/models/grave.ant2",1.0),AGVector4.new(0,0,0,0),40))
			#updateSurface
			sendAngel
		end
	end


	def shouldFight
		canFight
	end
private
	def sendAngel
		e=AntNewAngel.new
		e.setPos(getPos2D)
		getMap.insertEntity(e)
		getMap.endChange
	end

end