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


# AntAngle is an entity for displaying angels flying up
# when an AntMan dies. This is currently not supported, because
# there is no AntAngel mesh.
class AntAngel<AntRubyEntity

	def initialize
		super(AGVector2.new(0,0))
		setProvide("angel",true)
		@age=0
	end
	# simply move mesh up
	# FIXME:
	# this is very slow - should be removed and replaced by a MoveJob(3d)
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
end

# AntManBase is a base-class/module for all men (including heroes)
# it contains functionality for:
# * rowing a boat
# * sitting down
# * dying (and sending an angel)
# * shooting arrows
module AntManBase

	# call this to make a man sitdown
	# * animation is played
	# * setMeshState is needed for this
	def sitDown
		puts "sitDown"
		newRestJob(0.4)
		setMeshState("sitdown")
	end

	# overrides newFightJob from AntEntity
	# fighting distance is assigned 1 or 10 
	# depending on the man having a bow
	def newFightJob(p,target)
		if resource.get("bow")>0
			super(p,target,10)
		else
			super(p,target,1)
		end
	end

	def eventDefeated
		if is_a?(AntBoss)
			eventManDefeated(self)
		elsif @boss then
			@boss.eventManDefeated(self)
		end
	end
	def eventNoJob
		if @meshState=="dead"
			getMap.removeEntity(self)
		end
		super
	end

	def eventDie
		simDeath
		#eventDefeated
		newRestJob(20)
		setMeshState("dead")
		playSound("die")

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

	def checkOnWater(name)
		z=getMap.getPos(getPos2D).z
		if isOnWater
			# under water
			if name=~/sit/ and not isOnOpenWater
				name="stand"
				setOnWater(false)
			end
			if isOnOpenWater and haveBoat
				puts "onOpenWater"
				setOnWater(true)
				name="row"
			end
		else
			setOnWater(false)
		end
		name
	end

	def eventHitWaterMark(fromAbove)
		#setOnWater(fromAbove)
		if fromAbove
			if haveBoat
				setMeshState("row")
			else
				# stop job
				delJob
				p=getMap.getNextPlaceAbove(getPos2D,-0.2)
				newMoveJob(0,p,0)
			end
		else
			setOnWater(false)
			setPos(getMap.getPos(getPos2D))
			setMeshState(@origMeshState)
		end
	end

	def animationEvent(name)
		case name
			when "bow"
				arrow=AntArrow.new
				arrow.setPos(getPos3D+AGVector3.new(0,0,1))
				pos=@fightTarget.getPos3D+AGVector3.new(0,0,1)
				arrow.newMoveJob(0,pos,0) #@target.getPos2D,0)
				getMap.insertEntity(arrow)
		end
	end
protected
	def haveBoat
		resource.get("boat")>=1
	end

private
	def sendAngel
		e=AntAngel.new
		e.setPos(getPos2D)
		getMap.insertEntity(e)
		getMap.endChange
	end

end